/*******************************
** WAVPLAYER Maps Header File **
********************************/

#ifndef CBSE_WAVPLAYER_MAPS_HPP
#define CBSE_WAVPLAYER_MAPS_HPP

///Extra INFO tags
std::map <DWORD, std::string> infotags
{
    { DISP, "Display name" },
    { IARL, "Archival location" },
    { IBPM, "Beats Per Minute" },
    { ICMS, "Commissioned" },
    { ICRP, "Cropped" },
    { IDIM, "Dimensions" },
    { IDPI, "Dots Per Inch" },
    { IEDT, "Remixer" },
    { IENG, "Engineer" },
    { IKEY, "Keywords" },
    { ILGT, "Lightness" },
    { IMED, "Medium" },
    { INAM, "Name" },
    { IPLT, "Palette Setting" },
    { ISBJ, "Subject" },
    { ISHP, "Sharpness" },
    { ISRC, "Source" },
    { ISRF, "Source Form" },
    { ITCH, "Technician" },
    { IURL, "URL Link" }
};

///Mapping Error Codes to Error values
std::map <int, std::string> errorcodes
{
    { 0, "Normal Execution" },
    { 1, "Missing Configuration File" },
    { 2, "Invalid Configuration File" },
    { 3, "Invalid Command Line Arguments" },
    { 4, "File Not Found" },
};

#endif
