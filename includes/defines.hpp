/***********************************
** WAVPLAYER #defines Header File **
************************************/

#ifndef CBSE_WAVPLAYER_DEFS_HPP
#define CBSE_WAVPLAYER_DEFS_HPP

/**********************************************************/
/** Define FOURCC Codes for Microsoft WAVE format ChunkIDs*/
/**********************************************************/

#define RIFF 0x46464952		///< FOURCC Code of "RIFF"
#define WAVE 0x45564157		///< FOURCC Code of "WAVE"
#define FACT 0x54434146		///< FOURCC Code of "FACT"
#define FMT  0x20746D66		///< FOURCC Code of "fmt "
#define DATA 0x61746164		///< FOURCC Code of "data"
#define LIST 0x5453494c		///< FOURCC Code of "LIST"
#define INFO 0x4f464e49     ///< FOURCC Code of "INFO"

/**********************************************************/
/** Define FOURCC Codes for WAVE LIST/INFO tags           */
/**********************************************************/

#define DISP 0x50534944		///< FOURCC Code of "DISP"
#define IARL 0x4c524149		///< FOURCC Code of "IARL"
#define IBPM 0x4d504249     ///< FOURCC Code of "IBPM"
#define ICMS 0x534d4349		///< FOURCC Code of "ICMS"
#define ICRP 0x50524349		///< FOURCC Code of "ICRP"
#define IDIM 0x4d494449		///< FOURCC Code of "IDIM"
#define IDPI 0x49504449		///< FOURCC Code of "IDPI"
#define IEDT 0x54444549     ///< FOURCC Code of "IEDT"
#define IENG 0x474e4549		///< FOURCC Code of "IENG"
#define IKEY 0x59454b49		///< FOURCC Code of "IKEY"
#define ILGT 0x54474c49		///< FOURCC Code of "ILGT"
#define IMED 0x44454d49		///< FOURCC Code of "IMED"
#define INAM 0x4d414e49		///< FOURCC Code of "INAM"
#define IPLT 0x544c5049		///< FOURCC Code of "IPLT"
#define ISBJ 0x4a425349		///< FOURCC Code of "ISBJ"
#define ISHP 0x50485349		///< FOURCC Code of "ISHP"
#define ISRC 0x43525349		///< FOURCC Code of "ISRC"
#define ISRF 0x46525349		///< FOURCC Code of "ISRF"
#define ITCH 0x48435449		///< FOURCC Code of "ITCH"
#define IURL 0x4c525549		///< FOURCC Code of "IURL"
#define IART 0x54524149		///< FOURCC Code of "IART"
#define ICMT 0x544d4349		///< FOURCC Code of "ICMT"
#define ICOP 0x504f4349		///< FOURCC Code of "ICOP"
#define ICRD 0x44524349		///< FOURCC Code of "ICRD"
#define IGNR 0x524e4749		///< FOURCC Code of "IGNR"
#define INAM 0x4d414e49		///< FOURCC Code of "INAM"
#define IPRD 0x44525049		///< FOURCC Code of "IPRD"
#define ISFT 0x54465349		///< FOURCC Code of "ISFT"
#define ITRK 0x4b525449		///< FOURCC Code of "ITRK"

#define PCM_CODE   0x01     ///< Hex Code for PCM Format

/*************************************************/
/**Define ASCII Codes of Non Graphical Characters*/
/*************************************************/
#define ARROW       -32
#define LEFT         75
#define RIGHT        77
#define SPACEBAR     32
#define ENTER        13
#endif
