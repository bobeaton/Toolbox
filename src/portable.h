// portable.h Portability functions

#ifndef PORTABLE_H
#define PORTABLE_H

#ifdef wxTbx // 1.5.0gk For wx build, define MFC message styles to wx styles
#define MB_OK wxOK
#define MB_OKCANCEL wxOK|wxCANCEL
#define MB_YESNO wxYES_NO
#define MB_YESNOCANCEL wxYES_NO|wxCANCEL
#endif
int TbxMessageBox( const char* psz, unsigned nType ); 

/// void TbxAssert( const void* p );
// void TbxAssert( BOOL b );
// #define ASSERT TbxAssert
// xxx #define bEqual !strcmp

#endif  // PORTABLE_H
