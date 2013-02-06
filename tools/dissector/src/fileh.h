/*
 * fileh.h
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

#ifndef __FILEH_H__
#define __FILEH_H__
#include <string>
#include <config.h>


#define CHAR_ASCII 0


using namespace std;
extern "C" {
#include <epan/packet.h>
#include <epan/epan_dissect.h>
}

typedef struct _epan_dissect_t epan_dissect_t;

/* Current state of file. */
typedef enum {
    FILE_CLOSED, /* No file open */
    FILE_READ_IN_PROGRESS, /* Reading a file we've opened */
    FILE_READ_ABORTED, /* Read aborted by user */
    FILE_READ_DONE /* Read completed */
} file_state;

/* Character set for text search. */
typedef enum {
    SCS_ASCII_AND_UNICODE,
    SCS_ASCII,
    SCS_UNICODE
    /* add EBCDIC when it's implemented */
} search_charset_t;

//!  Class Dissector

/*!
This class represents the capture file. It contains the actual data and the methods that can be used to access them.
 * 
 */

class CaptureFile {
public:
    CaptureFile(string fileName);
    ~CaptureFile();
    bool open();
    int close();

    frame_data * readPacket(int *err, gchar **err_info, gint64* data_offset);



    union wtap_pseudo_header* getPseudoHeader();
    //bool printPacket(epan_dissect_t *edt);
    //void printNode(proto_node* tree);//fixme ma queste funzioni ci sono??
    //void printNode(proto_node* node,std::ostringstream* out);
    //void saveTree(epan_dissect_t *edt);


    //   dfilter_t* getRfcode(){return rfcode;}
    // column_info getCinfo(){return cinfo;}

    /** Returns the error message associated with the action just taken.
     * \return the message
     *
     */
    const string getErrorMessage() {
        return errorMessage;
    }

    const guchar* getPd();
private:
    void resetState();

    void fill_in_fdata(frame_data *fdata, gint64 offset);

    string errorMessage;
    string filename;

    guint32 cum_bytes;
    nstime_t first_ts;
    nstime_t prev_dis_ts;
    nstime_t prev_cap_ts;

    file_state state; /* Current state of capture file */
    gint64 f_datalen; /* Size of capture file data (uncompressed) */
    guint16 cd_t; /* File type of capture file */
    int lnk_t; /* Link-layer type with which to save capture */
    guint32 vers; /* Version.  For tcpdump minor is appended to major */
    guint32 count; /* Total number of frames */
    int displayed_count; /* Number of displayed frames */
    int marked_count; /* Number of marked frames */
    gboolean drops_known; /* TRUE if we know how many packets were dropped */
    guint32 drops; /* Dropped packets */
    nstime_t elapsed_time; /* Elapsed time */
    gboolean has_snap; /* TRUE if maximum capture packet length is known */
    int snap; /* Maximum captured packet length */
    wtap *wth; /* Wiretap session */
    dfilter_t *rfcode; /* Compiled read (display) filter program */
    gchar *dfilter; /* Display filter string */
    gboolean redissecting; /* TRUE if currently redissecting (cf_redissect_packets) */
    /* search */


    /* packet data */
    union wtap_pseudo_header pseudo_header; /* Packet pseudo_header */
    guint8 pd[WTAP_MAX_PACKET_SIZE]; /* Packet data */
    /* memory chunks have been deprecated in favor of the slice allocator,
     * which has been added in 2.10
     */
#if GLIB_CHECK_VERSION(2,10,0)

#else
    GMemChunk *plist_chunk; /* Memory chunk for frame_data structures */
#endif
    frame_data *plist; /* Packet list */
    frame_data *plist_end; /* Last packet in list */
    //	frame_data  *first_displayed; /* First frame displayed */
    //	frame_data  *last_displayed;  /* Last frame displayed */
    column_info cinfo; /* Column formatting information */
    frame_data *current_frame; /* Frame data for current frame */
    gint current_row; /* Row number for current frame */
    epan_dissect_t *edt; /* Protocol dissection for currently selected packet */
    //	field_info  *finfo_selected;	/* Field info for currently selected field */

};
#endif /* __FILEH_H__ */
