/*
 * model.cpp
 * Dissector
 *
 * Copyright (C) Evelina Agostini 2010 <evelina.agostini@gmail.com>
	 *
 * trunk is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
	 *
 * trunk is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <glib.h>
#include <model.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <util.h>
#include <iomanip>
#include <cstring>
extern "C" {
#include <epan/dissectors/packet-data.h>
#include <epan/packet.h>
#include <bits/stl_pair.h>
#include <dissector.h>
}

/** This method filter the elements of the tree based on the selected protocol layer.
 * 
 *
 * \return returns true if the node must be included in the ABNF representation
 * \param node the node to filter
 */

bool ProtocolFilter::filter(Node* node) {

	if (layer <= 2) return true;
	if (layer - 2 >= protocols.size()) return false;


	string nodeProtocol;

	size_t it = node->getProtocol().find('.');

	if (it != string::npos) {
		nodeProtocol = node->getProtocol().substr(0, it);
	} else nodeProtocol = node->getProtocol();

	for (int i = layer - 2; i < protocols.size(); i++) {
		if (nodeProtocol == protocols[i]) return true;
	}

	return false;

}

/** Destructor
 *
 *
 *
 */

Node::~Node() {
	multimap<int, Node*>::iterator children_iterator;
	for (children_iterator = children.begin(); children_iterator != children.end(); children_iterator++) {
		delete children_iterator->second;
	}
	children.clear();
}

/** This method print the element associated with the node and call itself on the children of the node, recursively.
 *
 * \return returns true if the node is visible (based on the filter)
 * \param out the pointer to the string stream on which the data will be written
 * \param f the pointer to a filter to determine if to write the information about this node
 * \param postfix a postfix to be appended to the name of the element
 * \param verbose a boolean value which determines if the method should print on the standard output
 */

bool Node::printNode(std::ostringstream* out, Filter* f, string postfix, bool verbose) {

	bool visible = false;

	bool passedFilter = (f == NULL || f->filter(this));
	visible = passedFilter;
	if (!children.empty()) {

		if (passedFilter || protocol == "root") {
			if (hidden) {
				*out << "; ";
				if (verbose) cout << "; ";
			}
			*out << name << postfix << " = ";
			if (verbose) cout << name << postfix << " = ";

			if (protocol == "root") {
				*out << "<read or send>";
				if (verbose) cout << "<read or send>";
			}
		}

		multimap<int, Node*>::iterator it;
		bool hasVisibleChildren = false;

		for (it = children.begin(); it != children.end(); it++) {

			if (passedFilter || f == NULL || f->filter(it->second)) {
				hasVisibleChildren = true;
				*out << " " << it->second->name;
				if (!it->second->isLeaf()) *out << postfix;

				if (verbose) {
					cout << " " << it->second->name;

					if (!it->second->isLeaf()) cout << postfix;

					cout << " (";
					cout << it->first << "-" << it->first + it->second->length - 1;

					cout << " - " << it->second->type.getName();
					cout << " - " << it->second->getProtocol();

					cout << ") ";

				}
			}
		}

		if (passedFilter || hasVisibleChildren) {
			if (verbose) cout << std::endl << std::endl;
			*out << std::endl;
			*out << std::endl;
		}

		for (it = children.begin(); it != children.end(); it++) {
			if (passedFilter)
				visible = it->second->printNode(out, NULL, postfix, verbose);
			else
				visible = it->second->printNode(out, f, postfix, verbose);
		}

	}
	return visible;
}

/** Costructor.
 *
 * \param packet the packet which is associated with this tree
 * \param node the wireshark structure that represents the root of the tree
 */

Tree::Tree(Packet* packet, proto_node* node) : root(new Node("packet", "root")), packet(packet) {


	addNodeToTree(node, root);

}

/** Destructor.
 *
 */

Tree::~Tree() {
	delete root;
}

/** This method is call to create and add a new node to the tree.
 *
 * \param protoNode the wireshark structure that represents the node
 * \param t the node among whose children we have to add the new node
 *
 */

void Tree::addNodeToTree(proto_node* protoNode, Node* t) {

	Node* node = NULL;
	if (protoNode) {
		if (protoNode->finfo && protoNode->finfo->hfinfo && protoNode->finfo->hfinfo->name) {


			if (!packet->display(protoNode)) {

				if (protoNode->next) {

					addNodeToTree(protoNode->next, t);

				}

			} else {

				pair < Node*, bool> n = packet->createNode(protoNode, false);

				node = n.first;

				t->addChild(node);

				t->setHidden(n.second);

			}
		}



		if (node == NULL) node = t;
		if (packet->doWeGetChildren(protoNode)) {
			int fine = 0;

			if (protoNode->finfo) {
				fine = protoNode->finfo->start + protoNode->finfo->length;
			}

			proto_node* protoChild = protoNode->first_child;

			string sn(protoChild->finfo->hfinfo->name);



			if (packet->display(protoChild)) {

				addNodeToTree(protoChild, node);
			} else {
				if (sn == "Frame") packet->loadPacketInfo(protoChild);

			}




			while (protoChild->next) {
				protoChild = protoChild->next;

				const char* n = protoChild->finfo->hfinfo->name;
				string sn(n);


				if (packet->display(protoChild)) {

					if (fine != 0 && protoChild->finfo->start > fine) { //In questo caso non e' davvero un figlio ma un fratello!
						addNodeToTree(protoChild, t);
					} else {
						addNodeToTree(protoChild, node);
					}
				}
			}

		} else {
			string sn(protoNode->finfo->hfinfo->name);

			string typeName = "";
			if (protoNode->finfo->value.ftype) typeName = protoNode->finfo->value.ftype->name;
			Type type(typeName, protoNode->finfo->hfinfo->type);




			pair < Node*, bool> n = packet->createNode(protoNode, true);
			Node* value = n.first;

			node->addChild(value);

			node->setHidden(n.second);
		}
	}
}

/** Destructor.
 *
 */


CaptureModel::~CaptureModel() {
	vector<Packet*>::iterator it;
	for (it = packets.begin(); it != packets.end(); it++) {
		delete (*it);

	}
	packets.clear();
}

/** This method is called to create the ABNF file (or files) that represents the packets in the capture files.
 *
 * \param output the path to the ABNF file to be created (or to the directory in which the files should be created)
 * \param multipleFiles a boolean value: if true it means that the user choose to create a single file for each packet, if false only
 * one file should be created with all the packets
 * \param verbose a boolean value which determines if the method should print on the standard output
 */


void CaptureModel::savePacketsAsAbnf(string output, bool multipleFiles, bool verbose) {
	vector<Packet*>::iterator it;
	ostringstream abnfString;

	if (!multipleFiles) {
		abnfString << "root =";
		if (verbose)cout << "root =";
		for (it = packets.begin(); it != packets.end(); it++) {
			abnfString << " " << (*it)->getName();
			if (verbose)cout << " " << (*it)->getName();
		}
		abnfString << endl << endl;
		if (verbose)cout << endl << endl;
	}


	it = packets.begin();
	if (verbose) cout << endl << endl;
	for (it = packets.begin(); it != packets.end(); it++) {

		if (verbose) {
			cout << "PACKET " << (*it)->getNumber() << endl << "-------------" << endl;
			(*it)->printPacketInfo();
			cout << "-------------" << endl;
		}

		if (multipleFiles) {
			ostringstream fileName2;
			fileName2 << output << "/packet" << (*it)->getNumber() << ".bnf";

			ostringstream abnfTreeString;
			(*it)->printTreeAsAbnf(&abnfTreeString, true, verbose);
			cout << fileName2.str() << endl;
			if (fileName2.str() != "") {
				ofstream myfile(fileName2.str().c_str());
				if (myfile.is_open()) {

					myfile << abnfTreeString.str() << endl;
					myfile.close();
				} else Dissector::printError(string("Unable to open file ") + fileName2.str());
			}


		} else {
			(*it)->printTreeAsAbnf(&abnfString, false, verbose);
			abnfString << endl;
		}

	}

	if (!multipleFiles) {



		if (output != "") {
			ofstream myfile(output.c_str());
			if (myfile.is_open()) {

				myfile << abnfString.str() << endl;
				myfile.close();
			} else Dissector::printError("Unable to open file " + output);
		}

	}
}

/** Destructor.
 *
 */


Packet::~Packet() {
	epan_dissect_free(edt);
	delete tree;
}

/**
 * Find the data source for a specified field, and return a pointer
 * to the data in it. Returns NULL if the data is out of bounds.
 */
const guint8* Packet::get_field_data(GSList *src_list, field_info *fi) {
	GSList *src_le;
	data_source *src;
	tvbuff_t *src_tvb;
	gint length, tvbuff_length;




	for (src_le = src_list; src_le != NULL; src_le = src_le->next) {
		src = ((data_source*) (src_le->data));
		src_tvb = src->tvb;
		if (fi->ds_tvb == src_tvb) {
			/*
			 * Found it.
			 *
			 * XXX - a field can have a length that runs past
			 * the end of the tvbuff.  Ideally, that should
			 * be fixed when adding an item to the protocol
			 * tree, but checking the length when doing
			 * that could be expensive.  Until we fix that,
			 * we'll do the check here.
			 */
			tvbuff_length = tvb_length_remaining(src_tvb,
			    fi->start);
			if (tvbuff_length < 0) {
				return NULL;
			}
			length = fi->length;
			if (length > tvbuff_length)
				length = tvbuff_length;
			return tvb_get_ptr(src_tvb, fi->start, length);
		}
	}
	//	g_assert_not_reached();
	return NULL; /* not found */
}

/** Gers the hexadecimal representation of the value of the field.
 *
 */


const gchar* Packet::get_field_hex_value(GSList* src_list, field_info *fi) {
	const guint8 *pd;

	if (!fi->ds_tvb)
		return NULL;

	if (fi->length > tvb_length_remaining(fi->ds_tvb, fi->start)) {
		return "field length invalid!";
	}

	/* Find the data for this field. */
	pd = get_field_data(src_list, fi);

	gint length, tvbuff_length;

	if (pd) {
		int i;
		gchar* buffer;
		gchar* p;
		int len;
		const int chars_per_byte = 2;

		len = chars_per_byte * fi->length;
		buffer = ep_alloc_array(gchar, len + 1);
		buffer[len] = '\0'; /* Ensure NULL termination in bad cases */
		p = buffer;
		/* Print a simple hex dump */
		for (i = 0; i < fi->length; i++) {
			g_snprintf(p, chars_per_byte + 1, "%02x", pd[i]);
			p += chars_per_byte;
		}
		return buffer;
	} else {
		return NULL;
	}
}

/** This method determines the value for a field.
 *
 * \return a pair in which the first element is the value and the second element is a boolean value that says if it should be showed.
 * \param fi the wireshark structure that contains the info about the field
 */


pair<std::string, bool> Packet::getNodeFieldValue(field_info* fi) {
	ostringstream rawValue;
	bool ok = true;
	// string    s= getNodeFieldPrintableValue(fi);
	if (fi) {
		rawValue << setfill('0');
		switch (fi->hfinfo->type) {
			case FT_NONE: /* used for text labels with no value */
			{
				const gchar* hexValue = get_field_hex_value(edt->pi.data_src, fi);
				if (hexValue) {
					rawValue << Util::getABNFString(hexValue);
				}
				else {
					rawValue << "WARNING - missing " << fi->length * 4 << " bit value: " << "\"" << getNodeFieldAsString(fi) << "\"";
					ok = false;
				}
				break;
			}
			case FT_PROTOCOL:
			{
				rawValue << "WARNING - missing value (FTPROTOCOL: " << fi->length * 4 << "bit): " << "\"" << getNodeFieldAsString(fi) << "\". This field shouldn't be read as a leaf."; //FIXME
				ok = false;
				break;
			}
				case FT_BOOLEAN: /* TRUE and FALSE come from <glib.h> */
			{
				rawValue << "%b" << fi->value.value.uinteger;
				break;
			}
			case FT_UINT8:
			{
				rawValue << "%x" << hex << setw(fi->length * 2) << fi->value.value.uinteger;
				break;
			}

			case FT_UINT16:
			{
				rawValue << "%x" << hex << setw(fi->length * 2) << fi->value.value.uinteger;
				break;
			}

			case FT_UINT24:
			{
				rawValue << "%x" << hex << setw(fi->length * 2) << fi->value.value.uinteger;
				break;
			}
			case FT_UINT32:
			case FT_FRAMENUM: /* a UINT32: but if selected lets you go to frame with that numbe */
			{
				rawValue << "%x" << hex << setw(fi->length * 2) << fi->value.value.uinteger;
				break;
			}
			case FT_UINT64:
			{
				rawValue << "%x" << hex << setw(fi->length * 2) << fi->value.value.uinteger;
				break;
			}
			case FT_INT64:
			{
				rawValue << "%x" << hex << setw(fi->length * 2) << fi->value.value.uinteger;
				break;
			}
			case FT_INT8:
			{
				// rawValue << "%d" << fi->value.value.sinteger;
				rawValue << "%x" << hex << setw(fi->length * 2) << fi->value.value.sinteger;
				break;
			}
			case FT_INT16:
			{
				// rawValue << "%d" << fi->value.value.sinteger;
				rawValue << "%x" << hex << setw(fi->length * 2) << fi->value.value.sinteger;
				break;
			}
			case FT_INT24:
			{
				// rawValue << "%d" << fi->value.value.sinteger;
				rawValue << "%x" << hex << setw(fi->length * 2) << fi->value.value.sinteger;
				break;
			}
			case FT_INT32:
			{
				// rawValue << "%d" << fi->value.value.sinteger;
				rawValue << "%x" << hex << setw(fi->length * 2) << fi->value.value.sinteger;
				break;
			}
			case FT_ABSOLUTE_TIME:
			case FT_RELATIVE_TIME:
			{
				long t = fi->value.value.time.nsecs + (fi->value.value.time.secs)*1000000;
				rawValue << "%d" << t;
				break;
			}
			case FT_STRING:
			{
				// rawValue << "%x" << get_field_hex_value(edt->pi.data_src,fi);
				string stringValue(fi->value.value.string);
				const gchar* hexValue = get_field_hex_value(edt->pi.data_src, fi);
				//                    if (!hexValue || Util::asciiToHex(fi->value.value.string).compare(hexValue)==0)
				//                        {
				//                        rawValue << "\"" << fi->value.value.string << "\"";
				//
				//                    }
				//                    else
				{
					rawValue << Util::getABNFString(hexValue); //<<  " ; \"" << fi->value.value.string << "\"";
				}
				//                    else
				//                        rawValue << "%x" << hexValue << " ; \"" << fi->value.value.string << "\"";
				break;
			}
			case FT_ETHER:
			{
				rawValue << "%x" << Util::ethToHex(getNodeFieldAsString(fi));
				break;
			}
			case FT_BYTES:
			{
				rawValue << "%x" << Util::dataToHex(getNodeFieldAsString(fi));
				//      rawValue << "%x" << get_field_hex_value(edt->pi.data_src,fi);
				break;
			}
			case FT_UINT_BYTES:
			{
				rawValue << "%x" << Util::dataToHex(getNodeFieldAsString(fi));
				//  rawValue << "%x" << get_field_hex_value(edt->pi.data_src,fi);
				break;
			}
			case FT_IPv4:
			{
				rawValue << "%x" << hex << setw(fi->length * 2) << fi->value.value.ipv4.addr;
				break;
			}
				case FT_FLOAT: //FIXME quando ci puo essere un double??
				case FT_DOUBLE:
				case FT_IPv6://FIXME
				case FT_IPXNET:
				case FT_PCRE: /* a compiled Perl-Compatible Regular Expression object */
					case FT_GUID: /* GUID: UUID */
					case FT_OID: /* OBJECT IDENTIFIER */
					case FT_NUM_TYPES: /* last item number plus one */
					case FT_STRINGZ: /* for use with proto_tree_add_item() */
					case FT_EBCDIC: /* for use with proto_tree_add_item() */
					case FT_UINT_STRING: /* for use with proto_tree_add_item() */
			{
				rawValue << "WARNING - missing " << fi->length * 4 << " bit value: " << "\"" << getNodeFieldAsString(fi) << "\" (Unsupported type";
				if (fi->value.ftype)
					rawValue << ": " << fi->value.ftype->name;
				rawValue << ")";
				ok = false;
				break;
			}



			default:
			{
				rawValue << "%d" << fi->value.value.uinteger;

			}



		}

		if (ok && fi->rep && fi->rep->representation) {
			rawValue << " ;";

			rawValue << fi->rep->representation;
		}
	}



	string s = rawValue.str();
	return make_pair(s, ok);

}

/** This method create a string representation of the value of the field.
 *
 * \return a string that represents the value of the field
 * \param fi the wireshark structure that contains the info about the field
 */


const std::string Packet::getNodeFieldAsString(field_info* fi) {
	ostringstream value;
	if (fi->hfinfo->id == hf_text_only) {
		/* Text label.
		 * Get the text */
		if (fi->rep) {
			value << fi->rep->representation;



			//           return value.str();
		} else {
			value << get_field_hex_value(edt->pi.data_src, fi);




			//            return value.str();
		}
	} else if (fi->hfinfo->id == proto_data) {
		/* Uninterpreted data, i.e., the "Data" protocol, is
		 * printed as a field instead of a protocol. */
		value << get_field_hex_value(edt->pi.data_src, fi);

		//            return value.str();
	} else {
		/* Normal protocols and fields */
		gchar *dfilter_string;
		size_t chop_len;

		switch (fi->hfinfo->type) {
			case FT_PROTOCOL:
				/* Print out the full details for the protocol. */
				if (fi->rep) {
					value << fi->rep->representation;


					//                                        return value.str();
				} else {
					/* Just print out the protocol abbreviation */
					value << fi->hfinfo->abbrev;

					//                                          return value.str();
				}
			case FT_NONE:
				/* Return "1" so that the presence of a field of type
				 * FT_NONE can be checked when using -T fields */
			{
				value << "1";
				//                                             return value.str();

			}
			default:
				/* XXX - this is a hack until we can just call
				 * fvalue_to_string_repr() for *all* FT_* types. */
				dfilter_string = proto_construct_match_selected_string(fi, edt);
				if (dfilter_string != NULL) {
					chop_len = strlen(fi->hfinfo->abbrev) + 4; /* for " == " */

					/* XXX - Remove double-quotes. Again, once we
					 * can call fvalue_to_string_repr(), we can
						 * ask it not to produce the version for
					 * display-filters, and thus, no
					 * double-quotes. */
					if (dfilter_string[strlen(dfilter_string) - 1] == '"') {
						dfilter_string[strlen(dfilter_string) - 1] = '\0';
						chop_len++;
					}

					value << &(dfilter_string[chop_len]);




					//                                                    return value.str();

				} else {
					value << get_field_hex_value(edt->pi.data_src, fi);

					//                                                  return value.str();

				}
		}
	}
	return value.str();
}



//const std::string Packet::getNodeFieldPrintableValue(field_info* fi) {
//
//
//
//
//
//
//
//    ostringstream v;
//    v << getNodeFieldValue(fi).first << " (" << getNodeFieldAsString(fi) << " TYPE: " << fi->value.ftype->name << ")" << std::endl;
//    return v.str();
//
//
//}

/** This method load general informations about the packet from the wireshark structure that represents it.
 *
 * 
 * \param node the wireshark structure that represents the root of the structure that contains the data of the packet
 */

void Packet::loadPacketInfo(proto_node* node) {

	proto_node* child = node->first_child;

	while (child) {
		const char* n = child->finfo->hfinfo->name;
		string name(n);

		string value = getNodeFieldAsString(child->finfo);



		if (name == "Protocols in frame")//FIXME esiste un modo migliore di capirlo che non sia leggere la stringa?
			loadProtocols(value);

		info.insert(make_pair(name, value));
		child = child->next;
	}


}

/** This method load the protocols contained in the packet.
 *
 *
 * \param p a string that contains a series of protocol abbreviations separated by ':'
 */



void Packet::loadProtocols(string p) {

	size_t it = p.find(':');

	while (it != string::npos) {
		string pr = p.substr(0, it);
		p = p.substr(it + 1, p.length() - 1);
		protocols.push_back(pr);
		it = p.find(':');

	}
	if (p.length() != 0)
		protocols.push_back(p);


	//create filter

	filter = new ProtocolFilter(protocols, layer);

}

/** This method prints general informations about the packet.
 *
 *
 *
 */

void Packet::printPacketInfo() {
	map<string, string>::iterator it;

	for (it = info.begin(); it != info.end(); it++) {
		cout << it->first << " = " << it->second << endl;
	}
}

/** This method determines if an element should be displayed.
 *
 *
 * \param node the wireshark structure that represents the root of the structure that contains the data of the packet
 */

bool Packet::display(proto_node* node) {

	const char* n = node->finfo->hfinfo->name;
	string sn(n);
	if (sn == "Frame" || (PROTO_ITEM_IS_HIDDEN(node)) || (PROTO_ITEM_IS_GENERATED(node)))/*FIXME Campi "HIDDEN" Queste sono info aggiuntive (presumibilmente generate da wireshark:
		    * ce ne facciamo qualcosa?*/
		return false;
	return true;
}

/** This method determines if we have to consider the children of a node, in the wireshark structure.
 * It bases the decision on what type of informations is carried by the node: we only want to represent the actual data
 * in the packet, we don't want to represent additional informations added by wireshark
 *
 *
 * \param node the wireshark structure that represents the root of the structure that contains the data of the packet
 */

bool Packet::doWeGetChildren(proto_node* node) {
	/*FIXME Se i nodi figli sono roba generata da wireshark allora
	 * prendo solo il valore
	 * Mi può servire questa roba?
	 * Esempio per il checksum guarda se è corretto o no
	 * e mette le "expert info" nei flag*/
	if (node->first_child) {
		bool checkLunghezze = false;
		int lunghezzaBabbo = 0;
		int sommaLunghezze = 0;
		if (node->finfo && node->finfo->hfinfo && node->finfo->hfinfo->type == FT_NONE) {
			/* e' una stringa...dovrei controllare se la somma delle lunghezze delle stringhe dei suoi figli
			 * corrisponde alla lunghezza della sua stringa (lo devo fare con le stringhe che rappresentano
			 * i valori esadecimali perche' nelle lunghezze numeriche possono mancare gli spazi! e lo stesso
			 * possono mancare nei valori dei figli! Es: HTTP GET)
			 * forse è un bug di wireshark 
			 * FIXME per il momento prendo il suo valore senza fare il controllo
			 * potrei perdermi dei delle "suddivisioni" cmq non dovrebbe causare errori*/
			const gchar* h = get_field_hex_value(edt->pi.data_src, node->finfo);
			if (h) {
				string hexValue(h);
				lunghezzaBabbo = hexValue.size();
				checkLunghezze = true;
			}
		}
		proto_node* child = node->first_child;
		bool ok = false;
		while (child && (!ok || checkLunghezze)) {
			if (checkLunghezze && child->finfo) {
				const gchar* h = get_field_hex_value(edt->pi.data_src, child->finfo);
				if (h) {
					string hexValue(h);
					sommaLunghezze = sommaLunghezze + hexValue.size();
				}
			}
			if (!ok) ok = !(PROTO_ITEM_IS_GENERATED(child)); //se tutti i figli sono roba generata da wireshark non li mettiamo nell'albero
			child = child->next;
		}
		if (sommaLunghezze != lunghezzaBabbo) return false;
		if (ok) {
			ftenum primitiveTypes[17] = {FT_IPv4, FT_BOOLEAN, FT_UINT8, FT_UINT16, FT_UINT24, FT_UINT32, FT_FRAMENUM, FT_UINT64, FT_INT64, FT_INT8, FT_INT16, FT_INT24, FT_INT32, FT_STRING, FT_ETHER, FT_BYTES, FT_UINT_BYTES};
			/* es: FT_ETHER La roba che mette come figli non sono dati successivi ma sono "pezzi" dell'indirizzo
			 * da cui si possono ricavare info*/
			for (int i = 0; i < 17; i++) {
				if (node->finfo && node->finfo->hfinfo) {
					if (primitiveTypes[i] == node->finfo->hfinfo->type)
						return false;
				}
			}
			return true;
		} else 
			return false;
	} else 
		return false;
}

/** This method create an object of the class Node with the data contained in a node in the wireshark structure.
 *
 * \sa Node
 * \returns a pair made by an object of the class Node and a boolean that is true if the node should be hidden
 * \param node the wireshark structure that represents the root of the structure that contains the data of the packet
 * \param leaf a boolan that says if the node is a leaf
 */

pair<Node*, bool> Packet::createNode(proto_node* node, bool leaf) {
	string typeName = "";
	if (node->finfo->value.ftype) typeName = node->finfo->value.ftype->name;
	Type type(typeName, node->finfo->hfinfo->type);

	string name = "";
	bool hidden = false;
	if (leaf) {

		pair < std::string, bool> v = getNodeFieldValue(node->finfo);
		pair < std::string, bool> b = getNodeFieldValue(node->parent->finfo);



		name = v.first;

		if (!v.second) {
			hidden = true;

		}

	} else {
		const char* n = node->finfo->hfinfo->name;
		string sn(n);
		sn = createName(node);
		name = Util::trimPlus(sn);

	}

	Node* value = new Node(name, node->finfo->start, node->finfo->length, type);
	value->setHidden(hidden);
	value->setProtocol(node->finfo->hfinfo->abbrev);


	return make_pair(value, hidden);
}

/** This method create the names that will be associated with a node in the tree,
 * based on the informations contained in the node in the wireshark structure..
 *
 *\return a string representing the name
 * \param node the wireshark structure that represents the root of the structure that contains the data of the packet
 */

string Packet::createName(proto_node* node) {
	const char* n = node->finfo->hfinfo->name;
	string sn(n);

	if (names.find(n) == names.end()) {
		names.insert(n);
		return n;
	} else {
		ostringstream name;
		while (node->parent && node->parent->finfo && node->parent->finfo->hfinfo->name) {


			name << node->parent->finfo->hfinfo->name;

			ostringstream nameTest;
			nameTest << name.str() << n;

			if (names.find(nameTest.str()) == names.end()) {
				names.insert(nameTest.str());
				return nameTest.str();
			} else node = node->parent;
		}


		int c = 1;
		bool ok = false;

		while (!ok) {
			ostringstream nameTest;
			nameTest << name.str() << n << c;

			if (names.find(nameTest.str()) == names.end()) {
				names.insert(nameTest.str());
				return nameTest.str();
			}
			c++;

		}
	}
}

/** This method create the tree associated with the packet.
 *
 *
 *  \sa Tree
 * 
 */


void Packet::createTree() {
	tree = new Tree(this, edt->tree);
}

/** This method print the contents of the packet in the ABNF representation.
 *
 *
 * \param abnfTreeString the pointer to the string stream on which the data will be written
 * \param multipleFiles a boolean value, if true, we should create a single file for each packet
 * \param verbose a boolean value which determines if the method should print on the standard output
 */


void Packet::printTreeAsAbnf(ostringstream* abnfTreeString, bool multipleFiles, bool verbose) {



	tree->printTree(abnfTreeString, filter, multipleFiles, verbose);


}
