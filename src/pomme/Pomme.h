#pragma once

#include <algorithm>
#include <sstream>

#include "PommeTypes.h"
#include "PommeEnums.h"
#include "PommeDebug.h"

#if _WIN32 // some windows header defines SetPort as a macro
	#undef SetPort
#endif

//-----------------------------------------------------------------------------
// Structure unpacking

#include "structpack.h"

//-----------------------------------------------------------------------------
// Big-endian transparent conversion

template<typename T> T BEByteSwap(T x) {
#if TARGET_RT_BIGENDIAN
	return x;
#else
	char* b = (char*)&x;
	std::reverse(b, b + sizeof(T));
	return x;
#endif
}

template<typename T> T ToBE(T x) { return BEByteSwap(x); }
template<typename T> T FromBE(T x) { return BEByteSwap(x); }

//-----------------------------------------------------------------------------
// PowerPC intrinsics

#define __fres(x) (1.0f/x)
#define __fabs(x) fabs(x)
#define __frsqrte(x) (1.0f/sqrtf(x))

//-----------------------------------------------------------------------------
// Source code compat

#define nil NULL

//-----------------------------------------------------------------------------
// FSSpec

OSErr FSMakeFSSpec(short vRefNum, long dirID, ConstStr255Param fileName, FSSpec* spec);

short FSpOpenResFile(const FSSpec* spec, char permission);

// Open a file's data fork
OSErr FSpOpenDF(const FSSpec* spec, char permission, short* refNum);

// Open a file's resource fork
OSErr FSpOpenRF(const FSSpec* spec, char permission, short* refNum);

OSErr FSpCreate(const FSSpec* spec, OSType creator, OSType fileType, ScriptCode scriptTag);

OSErr FSpDelete(const FSSpec* spec);

OSErr ResolveAlias(const FSSpec* spec, AliasHandle alias, FSSpec* target, Boolean* wasChanged);

OSErr FindFolder(short vRefNum, OSType folderType, Boolean createFolder, short* foundVRefNum, long* foundDirID);

OSErr DirCreate(short vRefNum, long parentDirID, ConstStr255Param directoryName, long* createdDirID);

//-----------------------------------------------------------------------------
// File I/O

OSErr FSRead(short refNum, long* count, Ptr buffPtr);

OSErr FSWrite(short refNum, long* count, Ptr buffPtr);

OSErr FSClose(short refNum);

OSErr GetEOF(short refNum, long* logEOF);

OSErr SetEOF(short refNum, long logEOF);

//-----------------------------------------------------------------------------
// Resource file management

// MoreMacintoshToolbox.pdf p174
OSErr ResError(void);

void UseResFile(short refNum);

// Gets the file reference number of the current resource file.
short CurResFile();

void CloseResFile(short refNum);

// Returns total number of resources of the given type
// in the current resource file only.
short Count1Resources(ResType);

Handle GetResource(ResType theType, short theID);

void ReleaseResource(Handle theResource);

void RemoveResource(Handle theResource);

void AddResource(Handle theData, ResType theType, short theID, const char* name);

void WriteResource(Handle theResource);

void DetachResource(Handle theResource);

long GetResourceSizeOnDisk(Handle);

long SizeResource(Handle);

//-----------------------------------------------------------------------------
// QuickDraw 2D

void SetRect(Rect* r, short left, short top, short right, short bottom);

PicHandle GetPicture(short PICTresourceID);

void DisposeGWorld(GWorldPtr offscreenGWorld);

// IM:QD:6-16
QDErr NewGWorld(
	GWorldPtr* offscreenGWorld,
	short pixelDepth,
	const Rect* boundsRect,
	void* junk1,	// CTabHandle cTable
	void* junk2,	// GDHandle aGDevice
	long junk3		// long flags
);

void GetGWorld(CGrafPtr* port, GDHandle* gdh);

void SetGWorld(CGrafPtr port, GDHandle gdh);

void SetPort(GrafPtr port);

void GetPort(GrafPtr* port);

void MoveTo(short h, short v);

void GetForeColor(RGBColor* rgb);

void ForeColor(long color);

void BackColor(long color);

void RGBForeColor(const RGBColor* color);

void PaintRect(const Rect* r);

void EraseRect(const Rect* r);

void LineTo(short h, short v);

void FrameRect(const Rect*);

short TextWidth(Ptr textBuf, short firstByte, short byteCount);

void DrawChar(char c);

void DrawString(ConstStr255Param s);

// IM:QD:7-44
void DrawPicture(PicHandle myPicture, const Rect* dstRect);

// IM:QD:6-31
PixMapHandle GetGWorldPixMap(GWorldPtr offscreenGWorld);

// IM:QD:6-38
Ptr GetPixBaseAddr(PixMapHandle pm);

void CopyBits(
	const PixMap* srcBits,
	PixMap* dstBits,
	const Rect* srcRect,
	const Rect* dstRect,
	short mode,
	void* maskRgn
);

//-----------------------------------------------------------------------------
// QuickDraw 2D extensions

Boolean Pomme_IsPortDirty(void);

void Pomme_SetPortDirty(Boolean dirty);

void Pomme_DumpPortTGA(const char* path);

//-----------------------------------------------------------------------------
// Misc

void ExitToShell();

void SysBeep(short duration);

void FlushEvents(short, short);

void NumToString(long theNum, Str255& theString);

//-----------------------------------------------------------------------------
// Input

void GetKeys(KeyMap);

Boolean Button(void);

//-----------------------------------------------------------------------------
// Memory: No-op

void MaxApplZone(void);

void MoreMasters(void);

Size CompactMem(Size);

// Compact system heap zone manually.
Size CompactMemSys(Size);

void PurgeMem(Size);

void PurgeMemSys(Size);

Size MaxMem(Size* grow);

void HNoPurge(Handle);

void HLock(Handle);

void HLockHi(Handle);

void NoPurgePixels(PixMapHandle);

// To prevent the base address for an offscreen pixel image from being moved
// while you draw into or copy from its pixel map.
Boolean LockPixels(PixMapHandle); // shall return true always

//-----------------------------------------------------------------------------
// Memory: Handle

Handle NewHandle(Size);

// Allocate prezeroed memory
Handle NewHandleClear(Size);

// Allocate temp memory
Handle TempNewHandle(Size, OSErr*);

Size GetHandleSize(Handle);

// Change the logical size of the relocatable block corresponding to a handle
void SetHandleSize(Handle, Size);

void DisposeHandle(Handle);

//-----------------------------------------------------------------------------
// Memory: Ptr

Ptr NewPtr(Size);

Ptr NewPtrSys(Size);

void DisposePtr(Ptr p);

//-----------------------------------------------------------------------------
// Memory: BlockMove

// Copies a sequence of bytes from one location in memory to another
void BlockMove(const void* srcPtr, void* destPtr, Size byteCount);

void BlockMoveData(const void* srcPtr, void* destPtr, Size byteCount);

//-----------------------------------------------------------------------------
// Time Manager

// Number of seconds elapsed since 1904-01-01 00:00
void GetDateTime(unsigned long* secs);

// Number of usecs elapsed since system startup
void Microseconds(UnsignedWide* microTickCount);

// Number of ticks elapsed since system startup (1 tick = approx. 1/60 of a second)
UInt32 TickCount();

//-----------------------------------------------------------------------------
// Mouse cursor

void HideCursor();

void InitCursor();

//-----------------------------------------------------------------------------
// Sound Manager

OSErr GetDefaultOutputVolume(long*);

OSErr SetDefaultOutputVolume(long);

OSErr SndNewChannel(SndChannelPtr* chan, short synth, long init, SndCallBackProcPtr userRoutine);

OSErr SndChannelStatus(SndChannelPtr chan, short theLength, SCStatusPtr theStatus);

OSErr SndDoImmediate(SndChannelPtr chan, const SndCommand* cmd);

OSErr GetSoundHeaderOffset(SndListHandle sndHandle, long* offset);

OSErr SndStartFilePlay(SndChannelPtr chan, short fRefNum, short resNum, long bufferSize, Ptr theBuffer, /*AudioSelectionPtr*/ void* theSelection, FilePlayCompletionUPP theCompletion, Boolean async);

OSErr SndPauseFilePlay(SndChannelPtr chan);

OSErr SndStopFilePlay(SndChannelPtr chan, Boolean quietNow);

NumVersion SndSoundManagerVersion();
