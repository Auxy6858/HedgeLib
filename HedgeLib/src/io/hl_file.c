#include "hedgelib/io/hl_file.h"
#include "hedgelib/hl_memory.h"
#include "../hl_in_assert.h"

#ifdef _WIN32
#include "../hl_in_win32.h"
#elif defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
#include "../hl_in_posix.h"
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
#else
#error "HedgeLib currently only supports Windows and POSIX-compliant platforms."
#endif

#ifdef _WIN32
typedef HANDLE HlINFileHandle;

static DWORD hlINWin32FileGetDesiredAccess(const HlFileMode mode)
{
    switch (mode & HL_FILE_MODE_MASK)
    {
    case HL_FILE_MODE_READ_WRITE:
        return GENERIC_READ | GENERIC_WRITE;

    case HL_FILE_MODE_READ:
        return GENERIC_READ;

    case HL_FILE_MODE_WRITE:
        return GENERIC_WRITE;

    default:
        return 0;
    }
}

static DWORD hlINWin32FileGetShareMode(const HlFileMode mode)
{
    if ((mode & HL_FILE_FLAG_SHARED) == 0) return 0;

    switch (mode & HL_FILE_MODE_MASK)
    {
    case HL_FILE_MODE_READ_WRITE:
        return FILE_SHARE_READ | FILE_SHARE_WRITE;

    case HL_FILE_MODE_READ:
        return FILE_SHARE_READ;

    case HL_FILE_MODE_WRITE:
        return FILE_SHARE_WRITE;

    default:
        return 0;
    }
}

static DWORD hlINWin32FileGetCreateOptions(const HlFileMode mode)
{
    switch (mode & HL_FILE_MODE_MASK)
    {
    case HL_FILE_MODE_READ:
        return OPEN_EXISTING;

    default:
    case HL_FILE_MODE_WRITE:
    case HL_FILE_MODE_READ_WRITE:
        return (mode & HL_FILE_FLAG_UPDATE) ?
            OPEN_ALWAYS : CREATE_ALWAYS;
    }
}
#elif defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
typedef int HlINFileHandle;

static int hlINPosixFileGetFlags(const HlFileMode mode)
{
    switch (mode & HL_FILE_MODE_MASK)
    {
        case HL_FILE_MODE_READ:
            return O_RDONLY;

        case HL_FILE_MODE_WRITE:
            return O_WRONLY | O_CREAT | O_TRUNC;

        case HL_FILE_MODE_READ_WRITE:
            /* TODO: Is this all right? */
            return O_RDWR | ((mode & HL_FILE_FLAG_UPDATE) ?
                O_CREAT : O_TRUNC);

        default: return 0;
    }
}
#endif

typedef struct HlFile
{
    HlINFileHandle handle;
    size_t curPos;
}
HlFile;

HlResult hlFileOpen(const HlNChar* HL_RESTRICT filePath,
    HlFileMode mode, HlFile* HL_RESTRICT * HL_RESTRICT file)
{
    HlINFileHandle fileHandle;
    HlFile* hlFile;

#ifdef _WIN32
    SECURITY_ATTRIBUTES securityAttrs;

    /* Get desired access, share mode, and create options from HlFileMode. */
    const DWORD desiredAccess = hlINWin32FileGetDesiredAccess(mode);
    const DWORD shareMode = hlINWin32FileGetShareMode(mode);
    const DWORD createOptions = hlINWin32FileGetCreateOptions(mode);

    /* Allocate enough space for a HlFile. */
    hlFile = HL_ALLOC_OBJ(HlFile);
    if (!hlFile) return HL_ERROR_OUT_OF_MEMORY;

    /* Setup securityAttrs */
    securityAttrs.nLength = (DWORD)(sizeof(SECURITY_ATTRIBUTES));
    securityAttrs.lpSecurityDescriptor = 0;
    securityAttrs.bInheritHandle = 1;

    /* TODO: Support file paths longer than MAX_PATH characters. */

    /* Open the file at the given path */
    fileHandle =
#ifdef HL_IN_WIN32_UNICODE
        CreateFileW(
#else
        CreateFileA(
#endif
            filePath, desiredAccess,
            shareMode, &securityAttrs, createOptions,
            FILE_ATTRIBUTE_NORMAL, 0);

    /* Return result if we encountered an error. */
    if (fileHandle == INVALID_HANDLE_VALUE)
    {
        hlFree(hlFile);
        return hlINWin32GetResultLastError();
    }
#else
    /* Allocate enough space for a HlFile. */
    hlFile = HL_ALLOC_OBJ(HlFile);
    if (!hlFile) return HL_ERROR_OUT_OF_MEMORY;

    /* Open file and return if any errors were encountered. */
    fileHandle = open(filePath, hlINPosixFileGetFlags(mode),
        S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    /* Return result if we encountered an error. */
    if (fileHandle == -1)
    {
        hlFree(hlFile);
        return hlINPosixGetResultErrno();
    }
#endif
    
    /* Set pointer and return success. */
    hlFile->handle = fileHandle;
    hlFile->curPos = 0;
    *file = hlFile;

    return HL_RESULT_SUCCESS;

}

HlResult hlFileRead(HlFile* HL_RESTRICT file, size_t count,
    void* HL_RESTRICT buf, size_t* HL_RESTRICT readByteCount)
{
#ifdef _WIN32
    DWORD readBytes;
    BOOL succeeded;

    /* Ensure count can fit within a DWORD before casting to one. */
    HL_ASSERT(count <= ULONG_MAX);

    /* Read the given number of bytes from the file. */
    succeeded = ReadFile(file->handle,
        buf, (DWORD)count, &readBytes, 0);

    /* Increase file curPos. */
    file->curPos += readBytes;

    if (readByteCount)
    {
        /*
           The readByteCount pointer was provided; store the
           amount of bytes successfully read from the file into it
           and return the result.
        */
        *readByteCount = (size_t)readBytes;
        return (succeeded) ? HL_RESULT_SUCCESS :
            hlINWin32GetResultLastError();
    }
    else
    {
        /*
           The readByteCount pointer was not provided; return
           the result, also treating the amount of read bytes
           not being equal to count as an error.
        */
        if (!succeeded) return hlINWin32GetResultLastError();
        return (readBytes == count) ? HL_RESULT_SUCCESS :
            HL_ERROR_UNKNOWN;
    }
#else
    ssize_t readBytes;
    HlBool succeeded;

    /* Ensure count can fit within a ssize_t before casting to one. */
    HL_ASSERT(count <= SSIZE_MAX);

    /* Read the given number of bytes from the file. */
    readBytes = read(file->handle, buf, count);
    
    /* Set succeeded and act based on if the read succeeded or failed. */
    if ((succeeded = (readBytes != -1)))
    {
        /* Increase file curPos. */
        file->curPos += readBytes;
    }
    else
    {
        /* Set readBytes count to 0. */
        readBytes = 0;
    }

    if (readByteCount)
    {
        /*
           The readByteCount pointer was provided; store the
           amount of bytes successfully read from the file into it
           and return the result.
        */
        *readByteCount = (size_t)readBytes;
        return (succeeded) ? HL_RESULT_SUCCESS :
            hlINPosixGetResultErrno();
    }
    else
    {
        /*
           The readByteCount pointer was not provided; return
           the result, also treating the amount of read bytes
           not being equal to count as an error.
        */
        if (!succeeded) return hlINPosixGetResultErrno();
        return (readBytes == count) ? HL_RESULT_SUCCESS :
            HL_ERROR_UNKNOWN;
    }
#endif
}

HlResult hlFileWrite(HlFile* HL_RESTRICT file, size_t count,
    const void* HL_RESTRICT buf, size_t* HL_RESTRICT writtenByteCount)
{
#ifdef _WIN32
    DWORD writtenBytes;
    BOOL succeeded;

    /* Ensure count can fit within a DWORD before casting to one. */
    HL_ASSERT(count <= ULONG_MAX);

    /* Write the given number of bytes to the file. */
    succeeded = WriteFile(file->handle,
        buf, (DWORD)count, &writtenBytes, 0);

    /* Increase file curPos. */
    file->curPos += writtenBytes;

    if (writtenByteCount)
    {
        /*
           The writtenByteCount pointer was provided; store the
           amount of bytes successfully written to the file into it
           and return the result.
        */
        *writtenByteCount = (size_t)writtenBytes;
        return (succeeded) ? HL_RESULT_SUCCESS :
            hlINWin32GetResultLastError();
    }
    else
    {
        /*
           The writtenByteCount pointer was not provided; return
           the result, also treating the amount of written bytes
           not being equal to count as an error.
        */
        if (!succeeded) return hlINWin32GetResultLastError();
        return (writtenBytes == count) ? HL_RESULT_SUCCESS :
            HL_ERROR_UNKNOWN;
    }
#else
    ssize_t writtenBytes;
    HlBool succeeded;

    /* Ensure count can fit within a ssize_t before casting to one. */
    HL_ASSERT(count <= SSIZE_MAX);

    /* Write the given number of bytes to the file. */
    writtenBytes = write(file->handle, buf, count);
    
    /* Set succeeded and act based on if the write succeeded or failed. */
    if ((succeeded = (writtenBytes != -1)))
    {
        /* Increase file curPos. */
        file->curPos += writtenBytes;
    }
    else
    {
        /* Set writtenBytes count to 0. */
        writtenBytes = 0;
    }

    if (writtenByteCount)
    {
        /*
           The writtenByteCount pointer was provided; store the
           amount of bytes successfully written to the file and
           return the result.
        */
        *writtenByteCount = (size_t)writtenBytes;
        return (succeeded) ? HL_RESULT_SUCCESS :
            hlINPosixGetResultErrno();
    }
    else
    {
        /*
           The writtenByteCount pointer was not provided; return
           the result, also treating the amount of written bytes
           not being equal to count as an error.
        */
        if (!succeeded) return hlINPosixGetResultErrno();
        return (writtenBytes == count) ? HL_RESULT_SUCCESS :
            HL_ERROR_UNKNOWN;
    }
#endif
}

const static HlU8 hlINFileNulls[255] = { 0 };

HlResult hlFileWriteNulls(HlFile* HL_RESTRICT file,
    size_t amount, size_t* HL_RESTRICT writtenByteCount)
{
    HlResult result;
    if (amount > 255)
    {
        /* Allocate a buffer large enough to hold all of the nulls we want to write. */
        void* nulls = hlAlloc(amount);
        if (!nulls) return HL_ERROR_OUT_OF_MEMORY;

        /* Zero-out the allocated memory in the buffer. */
        memset(nulls, 0, amount);

        /* Write the nulls to the file and free the buffer. */
        result = hlFileWrite(file, amount, nulls, writtenByteCount);
        hlFree(nulls);
    }
    else
    {
        /* Write the given amount of nulls to the file using our static nulls buffer. */
        result = hlFileWrite(file, amount,
            hlINFileNulls, writtenByteCount);
    }

    return result;
}

HlResult hlFileWriteOff32(HlFile* HL_RESTRICT file,
    size_t basePos, size_t offVal, HlOffTable* HL_RESTRICT offTable)
{
    /* Compute offset. */
    const HlU32 off = (HlU32)(offVal - basePos);

    /* Add offset position to offset table if requested by user. */
    if (offTable)
    {
        HlResult result = HL_LIST_PUSH(*offTable, hlFileTell(file));
        if (HL_FAILED(result)) return result;
    }

    /* Write offset to file and return result. */
    return hlFileWrite(file, sizeof(off), &off, NULL);
}

HlResult hlFileWriteOff64(HlFile* HL_RESTRICT file,
    size_t basePos, size_t offVal, HlOffTable* HL_RESTRICT offTable)
{
    /* Compute offset. */
    const HlU32 off = (HlU32)(offVal - basePos);

    /* Add offset position to offset table if requested by user. */
    if (offTable)
    {
        HlResult result = HL_LIST_PUSH(*offTable, hlFileTell(file));
        if (HL_FAILED(result)) return result;
    }

    /* Write offset to file and return result. */
    return hlFileWrite(file, sizeof(off), &off, NULL);
}

HlResult hlFileAlign(HlFile* file, size_t stride)
{
    /* If stride is < 2, we don't need to align; return success. */
    size_t pos;
    if (stride-- < 2) return HL_RESULT_SUCCESS;

    /* Get the current file position. */
    pos = hlFileTell(file);

    /*
       Compute the closest position in the file that's aligned
       by the given stride, and jump to that position.
    */
    return hlFileJumpTo(file, ((pos + stride) & ~stride));
}

HlResult hlFilePad(HlFile* file, size_t stride)
{
    /* If stride is < 2, we don't need to pad; return success. */
    size_t pos;
    if (stride-- < 2) return HL_RESULT_SUCCESS;

    /* Get the current file position. */
    pos = hlFileTell(file);

    /*
       Compute the amount of nulls we need to write to align the
       file with the given stride, and write that many nulls.
    */
    return hlFileWriteNulls(file, ((pos + stride) & ~stride) - pos, 0);
}

HlResult hlFileClose(HlFile* file)
{
    /* Close the given file and return whether closing was successful or not. */
#ifdef _WIN32
    BOOL succeeded;
    
    if (!file) return HL_RESULT_SUCCESS;

    succeeded = CloseHandle(file->handle);
    hlFree(file);

    return (succeeded) ? HL_RESULT_SUCCESS :
        hlINWin32GetResultLastError();
#else
    int result;
    
    if (!file) return HL_RESULT_SUCCESS;

    result = close(file->handle);
    hlFree(file);

    return (result != -1) ? HL_RESULT_SUCCESS :
        hlINPosixGetResultErrno();
#endif
}

HlResult hlFileGetSize(HlFile* HL_RESTRICT file, size_t* HL_RESTRICT fileSize)
{
#ifdef _WIN32
    LARGE_INTEGER size;
    BOOL succeeded;

    /* Get the size of the given file. */
    succeeded = GetFileSizeEx(file->handle, &size);

    /*
       Store the file's size in the fileSize pointer, or
       0 if we failed to get the file's size.
    */
    *fileSize = (succeeded) ? (size_t)size.QuadPart : 0;

    /* Return result. */
    return (succeeded) ? HL_RESULT_SUCCESS :
        hlINWin32GetResultLastError();
#else
    struct stat st;
    if (fstat(file->handle, &st))
    {
        /* Store 0 in the fileSize pointer and return the error. */
        *fileSize = 0;
        return hlINPosixGetResultErrno();
    }
    else
    {
        /* Store the file's size in the fileSize pointer and return success. */
        *fileSize = st.st_size;
        return HL_RESULT_SUCCESS;
    }
#endif
}

HlResult hlFileLoad(const HlNChar* HL_RESTRICT filePath,
    void* HL_RESTRICT * HL_RESTRICT data, size_t* HL_RESTRICT dataSize)
{
    void* buf;
    HlFile* file;
    size_t fileSize;
    HlResult result;

    /* Open the file at the given file path. */
    result = hlFileOpen(filePath, HL_FILE_MODE_READ, &file);
    if (HL_FAILED(result)) return result;

    /* Get the file's size. */
    result = hlFileGetSize(file, &fileSize);
    if (HL_FAILED(result)) goto failed_pre_alloc;

    /* Allocate a buffer large enough to hold the entire contents of the file. */
    buf = hlAlloc(fileSize);
    if (!buf)
    {
        result = HL_ERROR_OUT_OF_MEMORY;
        goto failed_pre_alloc;
    }

    /* Read all bytes from the file into the buffer. */
    result = hlFileRead(file, fileSize, buf, NULL);
    if (HL_FAILED(result)) goto failed;

    /* Close the file. */
    hlFileClose(file);

    /* Set pointers and return success. */
    *data = buf;
    if (dataSize) *dataSize = fileSize;

    return HL_RESULT_SUCCESS;

failed:
    hlFree(buf);

failed_pre_alloc:
    hlFileClose(file);
    return result;
}

HlResult hlFileSave(const void* HL_RESTRICT data,
    size_t count, const HlNChar* HL_RESTRICT filePath)
{
    HlFile* file;
    HlResult result;

    /* Open the file at the given file path, creating it if it doesn't yet exist. */
    result = hlFileOpen(filePath, HL_FILE_MODE_WRITE, &file);
    if (HL_FAILED(result)) return result;

    /* Write all bytes in the buffer to the file, close the file, and return. */
    result = hlFileWrite(file, count, data, 0);
    hlFileClose(file);

    return result;
}

#ifdef _WIN32
static DWORD hlINWin32FileGetMoveMethod(const HlSeekMode mode)
{
    switch (mode)
    {
    default:
    case HL_SEEK_MODE_BEG:
        return FILE_BEGIN;

    case HL_SEEK_MODE_CUR:
        return FILE_CURRENT;

    case HL_SEEK_MODE_END:
        return FILE_END;
    }
}
#elif defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
static int hlINPosixFileGetSeekMethod(const HlSeekMode mode)
{
    switch (mode)
    {
    default:
    case HL_SEEK_MODE_BEG:
        return SEEK_SET;

    case HL_SEEK_MODE_CUR:
        return SEEK_CUR;

    case HL_SEEK_MODE_END:
        return SEEK_END;
    }
}
#endif

HlResult hlFileSeek(HlFile* file,
    long offset, HlSeekMode seekMode)
{
#ifdef _WIN32
    LARGE_INTEGER loffset, lcurPos;
    BOOL succeeded;

    /* Seek using the given parameters. */
    loffset.QuadPart = (LONGLONG)offset;

    succeeded = SetFilePointerEx(file->handle, loffset,
        &lcurPos, hlINWin32FileGetMoveMethod(seekMode));

    /* Set file curPos. */
    file->curPos = (size_t)lcurPos.QuadPart;

    /* Return result. */
    return (succeeded) ? HL_RESULT_SUCCESS :
        hlINWin32GetResultLastError();
#else
    /* Seek using the given parameters. */
    off_t curPos = lseek(file->handle, (off_t)offset,
        hlINPosixFileGetSeekMethod(seekMode));

    /* Return failure if seeking failed. */
    if (curPos == (off_t)-1) return hlINPosixGetResultErrno();

    /* Set file curPos and return success. */
    file->curPos = (size_t)curPos;
    return HL_RESULT_SUCCESS;
#endif
}

size_t hlFileTell(const HlFile* file)
{
    return file->curPos;
}

HlResult hlFileJumpTo(HlFile* file, size_t pos)
{
#ifdef _WIN32
    LARGE_INTEGER loffset, lcurPos;
    BOOL succeeded;

    /* Jump to the given position. */
    loffset.QuadPart = (LONGLONG)pos;

    succeeded = SetFilePointerEx(file->handle, loffset,
        &lcurPos, FILE_BEGIN);

    /* Set file curPos. */
    file->curPos = (size_t)lcurPos.QuadPart;

    /* Return result. */
    return (succeeded) ? HL_RESULT_SUCCESS :
        hlINWin32GetResultLastError();
#else
    /* Jump to the given position. */
    off_t curPos = lseek(file->handle, (off_t)pos, SEEK_SET);

    /* Return failure if seeking failed. */
    if (curPos == (off_t)-1) return hlINPosixGetResultErrno();

    /* Set file curPos and return success. */
    file->curPos = (size_t)curPos;
    return HL_RESULT_SUCCESS;
#endif
}

#ifndef HL_NO_EXTERNAL_WRAPPERS
HlResult hlFileWriteStringExt(HlFile* HL_RESTRICT file,
    const char* HL_RESTRICT str, size_t* HL_RESTRICT writtenByteCount)
{
    return hlFileWriteString(file, str, writtenByteCount);
}

HlResult hlFileJumpAheadExt(HlFile* file, long amount)
{
    return hlFileJumpAhead(file, amount);
}

HlResult hlFileJumpBehindExt(HlFile* file, long amount)
{
    return hlFileJumpBehind(file, amount);
}
#endif
