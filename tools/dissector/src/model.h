/*
 * model.h
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

#ifndef __MODEL_H__
#define __MODEL_H__


#include <vector>
#include <fileh.h>
#include <set>
#include <map>
#include <sstream>



using namespace std;


#define PROTO_ITEM_IS_CHECKSUM(proto_item)        \
	((proto_item) ? FI_GET_FLAG((proto_item)->finfo, PI_CHECKSUM) : 0)

/*TODO PROTO_ITEM_IS_CHECKSUM rende true SOLO SE il campo è un cecksum E non e' corretto:
 * infatti se il checksum e' corretto ci aggiunge solo l'informazione sotto forma di stringa "[correct]";
 * le info extra le aggiunge solo in caso di errore. Volendo si potrebbero modificare i dissector in modo
 * che invece vengano aggiunte sempre. Vediamo se serve a qualcosa eventualmente. Lo stesso vale per gli altri flag.*/
//PI_GROUP_MASK
//protocol field
//PI_CHECKSUM
//protocol field
//PI_SEQUENCE
//protocol field
//PI_RESPONSE_CO
//protocol field
//PI_REQUEST_COD
//data is undeco
//PI_UNDECODED
//protocol field
//PI_REASSEMBLE
//packet data is
//PI_MALFORMED

//!  Class Type

/*!
This class represents the type of a single element of the tree.
 *
 */

class Type {
public:

    /** Constructor
     *
     *
     */
    Type() : name(""), ftype(FT_NONE) {
    }

    /** Constructor
     *  \param n the name for the type
     * \param t the corresponding wireshark type
     *
     */
    Type(string n, ftenum t) : name(n), ftype(t) {
    }

    /** Returns the name of the Type.
     *  \return the name
     *
     *
     */
    const string getName() {
        return name;
    };

private:

    string name;
    ftenum ftype;

    /* possible ftypes:
     * FT_NONE  	/* used for text labels with no value */
    /* FT_PROTOCOL
    /* FT_BOOLEAN 	/* TRUE and FALSE come from <glib.h> */
    /* FT_UINT8
    /* FT_UINT16
    /* FT_UINT24 	/* really a UINT32  but displayed as 3 hex-digits if FD_HEX*/
    /* FT_UINT32
    /* FT_UINT64
    /* FT_INT8
    /* FT_INT16
    /* FT_INT24 	/* same as for UINT24 */
    /* FT_INT32
    /* FT_INT64
    /* FT_FLOAT
    /* FT_DOUBLE
    /* FT_ABSOLUTE_TIME
    /* FT_RELATIVE_TIME
    /* FT_STRING
    /* FT_STRINGZ 	/* for use with proto_tree_add_item() */
    /* FT_EBCDIC 	/* for use with proto_tree_add_item() */
    /* FT_UINT_STRING 	/* for use with proto_tree_add_item() */
    //	/*FT_UCS2_LE  */    /* Unicode  2 byte  Little Endian     */
    /* FT_ETHER
    /* FT_BYTES
    /* FT_UINT_BYTES
    /* FT_IPv4
    /* FT_IPv6
    /* FT_IPXNET
    /* FT_FRAMENUM 	/* a UINT32  but if selected lets you go to frame with that numbe */
    /* FT_PCRE 		/* a compiled Perl-Compatible Regular Expression object */
    /* FT_GUID 		/* GUID  UUID */
    /* FT_OID 			/* OBJECT IDENTIFIER */
    /* FT_NUM_TYPES  /* last item number plus one */

};


class Node;


//!  Class Filter

/*!
This class represents a filter which can be used to select which packet to put in the ABNF file. This is an abstract class.
 *
 */


class Filter {
public:
    /** This method act as a filter for the Node to be displayed.
     *\return a boolan value: if true the node should be displayed, if false it shouldn't
     * \param node the node to filter
     *
     */
    virtual bool filter(Node* node) = 0;
};


//!  Class Node

/*!
This class represents a node in the tree: there is one node for each element in the dissection structure.
 * The dissection structure has the structure of a tree, but this class is needed in order to create an OO interface to handle
 * wireshark structures.
 *
 */


class Node {
public:

        /** Constructor
     *
     *
     */
    Node() : name(""), children(), hidden(false), position(0), length(0), type(), protocol("") {
    };

        /** Constructor
     *\param n the name for the node
     * \param p the position in the packet of the element represented in the node
     * \param l the lenght in the packet of the element represented in the node
     * \param t the type of the node
     * \param h a boolean value that says if the node should be hidden
     *
     */
    Node(string n, int p, int l, Type t, bool h = false) : name(n), children(), hidden(h), position(p), length(l), type(t), protocol("") {
    };

        /** Constructor
     *\param n the name for the node
     * \param p the protocol whose dissector created this element (this element corresponds to a field of a message of that protocol)
     *
     */
    Node(string n, string p) : name(n), children(), hidden(false), position(0), length(0), type(), protocol(p) {
    };


    ~Node();

    bool printNode(std::ostringstream* out, Filter* f, string postfix, bool verbose);

    /** This method add a child to the node
     *\param node the Node object to add as a child
     */
    void addChild(Node* node) {
        children.insert(make_pair(node->position, node));
    }

    /** Returns the position of the element in the packet
     *\return the position
     */
    const int getPosition() {
        return position;
    }

    /** Returns the name of the node
     *\return the name
     */
    const string getName() {
        return name;
    };

    /** Returns the protocol to which this element belongs
     *\return the abbreviation for the protocol
     */
    const string getProtocol() {
        return protocol;
    };

    /** Set the protocol to which this element belongs
     *\param p the position
     */

    void setProtocol(string p) {
        protocol = p;
    }

    /** Set the boolean values that says if the node should be hidden
     *\param h hidden
     */

    void setHidden(bool h) {
        hidden = h;
    }

    /** Returns true if the node is a leaf in the tree
     *\return true if it's a leaf
     */

    bool isLeaf() {
        return children.empty();
    }
private:
    string name;
    multimap<int, Node*> children;
    bool hidden;
    int position;
    int length;
    Type type;
    string protocol;


};

//!  Class ProtocolFilter

/*!
This class is a specialization of the Filter abstract class. It allows to filter elements based on the protocol layer.
 *
 */

class ProtocolFilter : public Filter {
public:

    /** Constructor
     *\param p a vector of strings representing the protocols in this packet (the abbreviations for them)
     * \param l the selected layer number
     */

    ProtocolFilter(vector<string> p, int l) : protocols(p), layer(l) {
    };

    bool filter(Node*);
private:
    vector<string> protocols;
    int layer;


};

class Tree;

//!  Class Packet

/*!
This class represents a single packet in the capture file. It contains informations about the packet and the actual data contained in it,
 * in a tree structure.
 *
 */

class Packet {
public:

    /** Constructor
     *\param e the wireshark structure containing the packet data
     * \param l the selected layer
     * \param n a sequential number identifying the packet in the capture file
     */

    Packet(epan_dissect_t* e, int l, int n) : edt(e), tree(NULL), info(), filter(NULL), layer(l), number(n), name("") {
        ostringstream names;
        names << "packet-" << number;
        name = names.str();
        createTree();
    }
    ~Packet();
    void printTreeAsAbnf(ostringstream* abnfTreeString, bool multipleFiles, bool verbose);
    pair<std::string, bool> getNodeFieldValue(field_info* fi);
    //   const string getNodeFieldPrintableValue(field_info* fi);
    void printPacketInfo();
    void loadPacketInfo(proto_node* node);
    void loadProtocols(string protocolsInFrame);
    bool display(proto_node* node);
    bool doWeGetChildren(proto_node* node);
    pair<Node*, bool> createNode(proto_node* node, bool leaf);

    /** Returns the sequential number identifying the packet in the capture file
     *\return the number of the packet
     */

    const int getNumber() {
        return number;
    }

    /** Returns the name of the packet, it should be made by "packet-" followed by the packet number
     *\return the name of the packet
     */
    const string getName() {
        return name;
    }
private:
    const guint8* get_field_data(GSList *src_list, field_info *fi);
    const gchar* get_field_hex_value(GSList* src_list, field_info *fi);

    const string getNodeFieldAsString(field_info* fi);



    void createTree();

    string createName(proto_node* node);

    epan_dissect_t* edt;
    Tree* tree;
    map<string, string> info;

    set<string> names;

    vector<string> protocols;
    Filter* filter;
    int layer;
    int number;
    string name;
};

//!  Class Tree

/*!
This class contains a tree structure representing the field of a single packet.
 * It contains the methods to create the tree and to print it.
 *
 */

class Tree {
public:

    /** Constructor
     *
     */
    Tree() : root(new Node("root", "root")) {
    }

    Tree(Packet* p, proto_node* node);
    void addNodeToTree(proto_node* node, Node* tree);
    ~Tree();

    /** This method creates an ABNF representation of the tree.
     *\param out string stream to which we print the data
     * \param filter an object of the class Filter, to filter the data
     * \param verbose a boolean that is true if we want verbose output
     * \param multipleFiles a boolean value: if true it means that the user choose to create a single file for each packet, if false only
     * one file should be created with all the packets
     */
    void printTree(std::ostringstream* out, Filter* filter, bool multipleFiles, bool verbose) {
        ostringstream postfix;
        if (!multipleFiles)
            postfix << "-" << packet->getNumber();

        ostringstream tree;
        if (root->printNode(&tree, filter, postfix.str(), verbose) == false) {
            *out << "; ";
        }
        *out << tree.str();

    };


private:


    Node* root;
    Packet* packet;
};

//!  Class CaptureModel

/*!
This class represents the model used to mantain the data extracted from the caputure file.
 * It contains the methods used to create that data and to transform it into the ABNF file.
 *
 */


class CaptureModel {
public:

    /** Constructor
     *  \param l the layer for the filter
     * \param v a boolean for verbose output
     *  
     */
    CaptureModel(int l, bool v = false) : packets(), layer(l), verbose(v) {
    };
    ~CaptureModel();

    /** This method create an object of class Packet with the data contained in the structure made by wireshark.
     * \sa Packet
     *  \param e the structure containing the data
     * \param number a sequential number identifying the packet in the capture file
     *
     */
    void createPacket(epan_dissect_t* e, int number) {
        packets.push_back(new Packet(e, layer, number));
    }

    void savePacketsAsAbnf(string output, bool multipleFiles, bool verbose);

    /** Set the layer for the filter
     *  \param l the layer value
     *
     */
    void setlayer(int l) {
        layer = l;
    }



private:
    vector<Packet*> packets;
    int layer;
    bool verbose;
};

#endif
