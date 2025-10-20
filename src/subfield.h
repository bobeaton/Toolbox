// subfield.h  Sub-fields: Character styles in Standard Format

#ifndef SUBFIELD_H
#define SUBFIELD_H

#include "crecpos.h"


class CSubFieldRecPos : public CRecPos  // Hungarian: rps
{
private:
    CMarker* m_pmkrSubField;
        // A subfield is an element distinguished from the surrounding
        // field contents because it differs in language or function.
        // A subfield consists of its text contents enclosed in
        // matching begin and end tags, which indicate its marker.
        // In a file the subfield tag delimiters are curly braces,
        // but when the contents of fields are read into the program
        // these delimiters are converted to character codes 1 and 2.
        //
        // Example of a difference in language:
        // \ng Replicated form from root {fv}ahw{-fv}.
        // Example of same language, but a difference in function:
        // \ee This is {ew}not{-ew} limited to coconut groves
        //     but is used for mangoes, etc.
        //
        // Examples of the parts:
        // Subfield:   {fv}ahw{-fv}
        // Contents:   ahw
        // Marker:     fv
        // Begin tag:  {fv}
        // End tag:    {-fv}
        // Delimiters: {}

public:
    CSubFieldRecPos(int iCharPar = 0, CField* pfldPar = NULL,
            CRecord* precPar = NULL);
    
    CMarker* pmkr() const;
        // Returns the field or subfield marker of the substring
        // beginning at this position and ending at the next tag
        // or at the end of the field.
    
    BOOL bMoveRightPastSubFieldTags();
        // Returns whether there is a subfield tag at this position.
        // If there is, this position moves to the right past it
        // (and past any other adjacent tags).
    BOOL bMoveLeftPastSubFieldTags();
        // Returns whether this position is just after a subfield tag.
        // If it is, this position moves to the left past it
        // (and past any other adjacent tags).

    // bMoveRightPastViewSubString
    BOOL bMovePastNextViewSubString(CSubFieldRecPos* prpsBegin,
            CSubFieldRecPos* prpsEnd);
        // Returns whether there is a view substring at this position.
        // If there is, this position moves to the right past it
        // (i.e. past the tags following it, to the beginning
        // of the next line, or to the end of the field) and
        // the substring is returned as the interval [*prpsBegin,*prpsEnd);
        // otherwise neither this position nor the arguments are changed.
        // A view substring is a text element that can be drawn or
        // whose horizontal extent can be measured in one operation.

    static void s_ReadSubFields(CMarkerSet* pmkrset, char* pszContents);
        // Convert any curly brace subfield tag delimiters read from a file
        // to the internal codes used in the contents of field objects.
    static void s_WriteSubFields(ostream& ios, const Str8& sContents);
        // Write the contents of a field and convert the delimiters
        // of any subfield tags from the internal codes to curly braces.

    static void s_DecrementRefsToSubFieldMarkers(CField* pfld);
        // Decrement the references to any subfield markers in pfld
        // because the field is about to be deleted.
        // Both begin and end tags count as references to their markers.

private:
    void SetMarker();
        // Set the field or subfield marker at this position
        // when it is constucted and after it is moved.
    void SetSubFieldMarker(CMarker* pmkrSubField, BOOL bEndMarker,
            BOOL bMovingRight);
        // Set or clear this position's subfield marker
        // because it is moving past a subfield tag.

    BOOL bMoveRightToNextSubFieldTag(Length* plenTag,
            CMarker** ppmkrSubField, BOOL* pbEndMarker);
        // Returns whether there is a subfield tag at or after this position.
        // If there is, this position moves to it and its length,
        // marker, and whether it is a begin or end tag are returned;
        // otherwise neither this position nor the arguments are changed.
        // The caller can then continue to move to the right past the tag
        // by incrementing this position's character offset by *plenTag.

    BOOL bAtSubFieldTag() const;
        // Returns whether there is a subfield tag at this position.
    BOOL bAtSubFieldTag(Length* plenTag,
            CMarker** ppmkrSubField, BOOL* pbEndMarker) const;
        // Returns whether there is a subfield tag at this position.
        // If there is, its length, marker, and whether it is
        // a begin or end tag are returned.

    BOOL bAfterSubFieldTag() const;
        // Returns whether this position is just after a subfield tag.
    BOOL bAfterSubFieldTag(Length* plenTag,
            CMarker** ppmkrSubField, BOOL* pbEndMarker) const;
        // Returns whether this position is just after a subfield tag.
        // If there is, its length, marker, and whether it is
        // a begin or end tag are returned.

    static BOOL s_bAtExternalSubFieldTag(CMarkerSet* pmkrset,
            char** ppsz, CMarker** ppmkrSubField, BOOL* pbEndMarker);
        // Returns whether there is a subfield tag at this position.
        // Use this function when reading the fields from a file.
        // The caller should convert the external curly brace delimiters
        // to the internal character codes 1 and 2.
};  // class CSubFieldRecPos

#endif  // not SUBFIELD_H
