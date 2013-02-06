/*
 * fileh.cpp
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

#include <fileh.h>
#include <dissector.h>
#include <iostream>
#include <sstream>
#include <fstream>


//using namespace std;

struct wtap;

/** Constructor.
 *
 * \param name the string containg the path to the capture file
 * 
 */

CaptureFile::CaptureFile(string name) : filename(name),
plist(NULL),
plist_end(NULL),
wth(NULL),
rfcode(NULL),
dfilter(NULL),
has_snap(FALSE),
snap(WTAP_MAX_PACKET_SIZE),
count(0),
redissecting(FALSE),
errorMessage(""), cum_bytes(0) {


}

/** Destructor.
 *
 */

CaptureFile::~CaptureFile() {

}

/** This method cleans up the structures used by the wireshark library to handle the capture file.
 *
 */


int CaptureFile::close() {
    wtap_close(wth);
    cleanup_dissection();
    return 0;
}

/** This methods open the capture file. It needs to be called before accessing the data.
 *  \return a code that indicates the result of the action
 */

bool CaptureFile::open() {
    //wtap       *wth;
    gchar *err_info;
    int err;


    resetState();


    wth = wtap_open_offline(filename.c_str(), &err, &err_info, TRUE);
    if (wth == NULL) {
        if (err != 0) {
            /* Print a message noting that the read failed somewhere along the line. */
            switch (err) {

                case WTAP_ERR_UNSUPPORTED_ENCAP:
                {
                    ostringstream s;
                    s << "File " << filename << " has a packet with a network type that is not supported. (" << err_info << ")";
                    g_free(err_info);
                    errorMessage = s.str();
                    break;
                }
                case WTAP_ERR_CANT_READ:
                {
                    ostringstream s;
                    s << "An attempt to read from file " << filename << " failed for some unknown reason.";
                    errorMessage = s.str();
                    break;
                }
                case WTAP_ERR_SHORT_READ:
                {
                    ostringstream s;
                    s << "File " << filename << " appears to have been cut short in the middle of a packet.";
                    errorMessage = s.str();
                    break;
                }
                case WTAP_ERR_BAD_RECORD:
                {
                    ostringstream s;
                    s << "File " << filename << " appears to be damaged or corrupt.";
                    errorMessage = s.str();
                    g_free(err_info);
                    break;
                }
                default:
                {
                    ostringstream s;
                    s << "An error occurred while reading" << " file " << filename << ": " << wtap_strerror(err);
                    errorMessage = s.str();

                    break;
                }
            }

        }

        return false;
    }


    /* Initialize all data structures used for dissection. */
    init_dissection();

    /* We're about to start reading the file. */
    state = FILE_READ_IN_PROGRESS;

    //wth = wth;
    f_datalen = 0;



    cd_t = wtap_file_type(wth);
    count = 0;
    displayed_count = 0;
    marked_count = 0;
    drops_known = FALSE;
    drops = 0;
    snap = wtap_snapshot_length(wth);
    if (snap == 0) {
        /* Snapshot length not known. */
        has_snap = FALSE;
        snap = WTAP_MAX_PACKET_SIZE;
    } else
        has_snap = TRUE;
    nstime_set_zero(&elapsed_time);
    nstime_set_unset(&first_ts);
    nstime_set_unset(&prev_dis_ts);

#if GLIB_CHECK_VERSION(2,10,0)
#else
    /* memory chunks have been deprecated in favor of the slice allocator,
     * which has been added in 2.10
     */
    plist_chunk = g_mem_chunk_new("frame_data_chunk",
            sizeof (frame_data),
            FRAME_DATA_CHUNK_SIZE * sizeof (frame_data),
            G_ALLOC_AND_FREE);
    g_assert(plist_chunk);
#endif
    /* change the time formats now, as we might have a new precision */
    //cf_change_time_formats(cf);//FIXME ??





    return true;

}

/** This method resets the structures used by the wireshark library to handle the capture file.
 *  \return the result of the action
 */


void CaptureFile::resetState() {
    /* Die if we're in the middle of reading a file. */
    //	g_assert(state != FILE_READ_IN_PROGRESS); //FIXME controllo?

    if (wth) {
        wtap_close(wth);
        wth = NULL;
    }


#if GLIB_CHECK_VERSION(2,10,0)
    if (plist != NULL)
        g_slice_free_chain(frame_data, plist, next);
#else
    if (plist_chunk != NULL) {
        frame_data *fdata = plist;
        while (fdata) {
            g_strfreev(fdata->col_expr.col_expr);
            g_strfreev(fdata->col_expr.col_expr_val);
            fdata = fdata->next;
        }
        /* memory chunks have been deprecated in favor of the slice allocator,
         * which has been added in 2.10
         */
        g_mem_chunk_destroy(plist_chunk);
        plist_chunk = NULL;
    }
#endif
    if (rfcode != NULL) {
        dfilter_free(rfcode);
        rfcode = NULL;
    }
    plist = NULL;
    plist_end = NULL;
    //	cf_unselect_packet(cf);	/* nothing to select */
    //    first_displayed = NULL;
    //    last_displayed = NULL;
    //
    //    /* No frame selected, no field in that frame selected. */
    //    current_frame = NULL;
    //    current_row = 0;
    //    finfo_selected = NULL;

    /* Clear the packet list. */
    //	packet_list_freeze();
    //	packet_list_clear();
    //	packet_list_thaw();

    f_datalen = 0;
    count = 0;
    //nstime_set_zero(&elapsed_time);

    //  reset_tap_listeners();

    /* We have no file open. */
    state = FILE_CLOSED;

    //fileset_file_closed();
}

/** This method can be used to read a single packet from the capture file.
 * \param err the pointer to an integer which will be set in case of error
 * \param err_info the pointer to a gchar* string which will contain a message in case of error
 * \param data_offset the pointer to an integer which will contain the offset of the read packet
 *!
 */



frame_data* CaptureFile::readPacket(int *err, gchar **err_info, gint64* data_offset) {
    count++;
    gboolean ret;
    frame_data *fdata;
    /* Wireshark function extracts next packet for dissection */
    ret = wtap_read(wth, err, err_info, data_offset);
    fill_in_fdata(fdata, *data_offset);
    if (ret) return fdata;
    else return NULL;
}

/** This method can be used to fill a structure with the data of a packet, referred to with an offset.
 * \param fdata the pointer to the structure to be filled
 * \param offset the offset of the packet
 *!
 */

void CaptureFile::fill_in_fdata(frame_data *fdata, gint64 offset) {

    const struct wtap_pkthdr *phdr = wtap_phdr(wth);
    guint32 cum_bytes = 0;
    fdata->next = NULL;
    fdata->prev = NULL;
    fdata->pfd = NULL;
    fdata->num = count;
    fdata->pkt_len = phdr->len;
    cum_bytes += phdr->len;
    fdata->cum_bytes = cum_bytes;
    fdata->cap_len = phdr->caplen;
    fdata->file_off = offset;
    fdata->lnk_t = phdr->pkt_encap;
    fdata->abs_ts.secs = phdr->ts.secs;
    fdata->abs_ts.nsecs = phdr->ts.nsecs;
    fdata->flags.passed_dfilter = 0;
    //fdata->flags.encoding = PACKET_CHAR_ENC_CHAR_ASCII;
    fdata->flags.encoding = CHAR_ASCII;
    fdata->flags.visited = 0;
    fdata->flags.marked = 0;
    fdata->flags.ref_time = 0;
    fdata->color_filter = NULL;

    /* If we don't have the time stamp of the first packet in the
 capture, it's because this is the first packet.  Save the time
 stamp of this packet as the time stamp of the first packet. */
    if (nstime_is_unset(&first_ts)) {
        first_ts = fdata->abs_ts;
    }

    /* If we don't have the time stamp of the previous captured packet,
 it's because this is the first packet.  Save the time
 stamp of this packet as the time stamp of the previous captured
 packet. */
    if (nstime_is_unset(&prev_cap_ts)) {
        prev_cap_ts = fdata->abs_ts;
    }

    /* Get the time elapsed between the first packet and this packet. */
    nstime_delta(&fdata->rel_ts, &fdata->abs_ts, &first_ts);

    /* If it's greater than the current elapsed time, set the elapsed time
 to it (we check for "greater than" so as not to be confused by
 time moving backwards). */
    if ((gint32) elapsed_time.secs < fdata->rel_ts.secs
            || ((gint32) elapsed_time.secs == fdata->rel_ts.secs && (gint32) elapsed_time.nsecs < fdata->rel_ts.nsecs)) {
        elapsed_time = fdata->rel_ts;
    }

    /* If we don't have the time stamp of the previous displayed packet,
 it's because this is the first packet that's being displayed.  Save the time
 stamp of this packet as the time stamp of the previous displayed
 packet. */
    if (nstime_is_unset(&prev_dis_ts))
        prev_dis_ts = fdata->abs_ts;

    /* Get the time elapsed between the previous displayed packet and
 this packet. */
    nstime_delta(&fdata->del_dis_ts, &fdata->abs_ts, &prev_dis_ts);

    /* Get the time elapsed between the previous captured packet and
 this packet. */
    nstime_delta(&fdata->del_cap_ts, &fdata->abs_ts, &prev_cap_ts);
    prev_cap_ts = fdata->abs_ts;
}

union wtap_pseudo_header* CaptureFile::getPseudoHeader() {
    return wtap_pseudoheader(wth);
}

/** This method returns the pointer to the buffer of the current packet.
 *
 *
 */

const guchar* CaptureFile::getPd() {
    return wtap_buf_ptr(wth);
}

