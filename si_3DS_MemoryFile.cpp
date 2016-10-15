// si_3DS_Memory.cpp : Memory & File General Purpose class

// 11/May/2002 v1.00 - ing.davide.pizzolato@libero.it
// CxFile version 1.00 11/May/2002
//
// From space invaders opengl author: I use this class
// to load the 3DS Objects from resources (BINARY) like
// we do with files (i.e. with seek, read, getc functions)

#include "si_3DS_MemoryFile.h"		// For reading from file or resource
#include "si_Loading.h"				// <CUSTOM> Header file for update progress bar

//////////////////////////////////////////////////////////
CxFile::CxFile(long type)
{
	m_stream=NULL;
	m_StorageType = type; //0=file, 1=memory
	m_Position = m_Size = m_Edge = m_DontCloseOnExit = 0;
}
//////////////////////////////////////////////////////////
CxFile::CxFile(FILE* pFile)
{
	m_stream=pFile;
	m_StorageType = 0;
	m_Position = m_Size = m_Edge = 0;
	m_DontCloseOnExit = 1;
}
//////////////////////////////////////////////////////////
CxFile::CxFile(BYTE* pBuffer, DWORD size)
{
	m_stream=pBuffer;
	m_StorageType = 1;
	m_Position = 0;
	m_Size = m_Edge = size;
	m_DontCloseOnExit = 1;
}
void CxFile::Transfer(CxFile &from)
{
	m_stream = from.m_stream;
	m_StorageType = from.m_StorageType;
	m_Position = from.m_Position;
	m_Size = from.m_Size;
	m_Edge = from.m_Edge;
	m_DontCloseOnExit = from.m_DontCloseOnExit;

	from.m_stream = NULL;
}
//////////////////////////////////////////////////////////
CxFile::~CxFile()
{
	Close();
}
//////////////////////////////////////////////////////////
void* CxFile::Open(const char *filename, const char *mode)
{
	if (m_stream) return m_stream;
	switch(m_StorageType){
	case 0:
		return m_stream=fopen(filename, mode);
	case 1:
		{
			m_Position = m_Size = m_Edge = 0;
			return m_stream=malloc(0);
		}
	}
	return 0;
}
//////////////////////////////////////////////////////////
long CxFile::Close()
{
	if (m_DontCloseOnExit) return -1;
	if (m_stream==0) return false;
	switch(m_StorageType){
	case 0:
		{
			long m_err = fclose((FILE*)m_stream);
			m_stream=NULL;
			return m_err;
		}
	case 1:
		free(m_stream);
		m_stream=NULL;
		return 0;
	}
	return -1;
}
//////////////////////////////////////////////////////////
size_t CxFile::Read(void *buffer, size_t size, size_t count)
{
	if (m_stream==NULL) return 0;

	switch(m_StorageType){
	case 0:
		return fread(buffer, size, count, (FILE*)m_stream );
	case 1:
		{
			if (buffer==NULL) return 0;
			if (m_Position > m_Size) return 0;

			long nCount = count*size;
			if (nCount == 0) return 0;
			
			UINT nRead;
			if (m_Position + nCount > m_Size)
				nRead = (UINT)(m_Size - m_Position);
			else
				nRead = nCount;

			memcpy(buffer, (BYTE*)m_stream + m_Position, nRead);
			m_Position += nRead;

			return (size_t)(nRead/size);
		}
	}
	return 0;
}
//////////////////////////////////////////////////////////
size_t CxFile::Write(const void *buffer, size_t size, size_t count)
{
	if (m_stream==NULL) return 0;
	switch(m_StorageType){
	case 0:
		return fwrite(buffer, size, count, (FILE*)m_stream );
	case 1:
		{
			if (buffer==NULL) return 0;

			long nCount = count*size;
			if (nCount == 0) return 0;

			if (m_Position + nCount > m_Edge) Alloc(m_Position + nCount);

			memcpy((BYTE*)m_stream + m_Position, buffer, nCount);

			m_Position += nCount;

			if (m_Position > m_Size) m_Size = m_Position;
			
			return count;
		}
	}
	return 0;
}
//////////////////////////////////////////////////////////
long CxFile::Seek(long offset, int origin)
{
	if (m_stream==NULL) return 0;
	switch(m_StorageType){
	case 0:
		return fseek((FILE*)m_stream, offset, origin);
	case 1:
		{
			long lNewPos = m_Position;

			if (origin == SEEK_SET)		 lNewPos = offset;
			else if (origin == SEEK_CUR) lNewPos += offset;
			else if (origin == SEEK_END) lNewPos = m_Size + offset;
			else return -1;

			if (lNewPos < 0) return -1;

			m_Position = lNewPos;
			return 0;
		}
	}
	return -1;
}
//////////////////////////////////////////////////////////
long CxFile::Tell()
{
	if (m_stream==NULL) return -1;
	switch(m_StorageType){
	case 0:
		return ftell((FILE*)m_stream);
	case 1:
		return m_Position;
	}
	return -1;
}
//////////////////////////////////////////////////////////
long CxFile::GetPos(fpos_t *pos)
{
	if (m_stream==NULL) return -1;
	switch(m_StorageType){
	case 0:
		fgetpos((FILE*)m_stream,pos);
		return 0;
	case 1:
		*pos = (fpos_t) m_Position;
		return 0;
	}
	return -1;
}
//////////////////////////////////////////////////////////
long CxFile::Size()
{
	if (m_stream==NULL) return -1;
	switch(m_StorageType){
	case 0:
		{
			long pos,size;
			pos = ftell((FILE*)m_stream);
			fseek((FILE*)m_stream,0,SEEK_END);
			size = ftell((FILE*)m_stream);
			fseek((FILE*)m_stream,pos,SEEK_SET);
			return size;
		}
	case 1:
		return m_Size;
	}
	return -1;
}
//////////////////////////////////////////////////////////
long CxFile::Flush()
{
	if (m_stream==NULL) return -1;
	switch(m_StorageType){
	case 0:
		return fflush((FILE*)m_stream);
		break;
	case 1:
		return 0;
	}
	return -1;
}
//////////////////////////////////////////////////////////
long CxFile::Eof()
{
	if (m_stream==NULL) return -1;
	switch(m_StorageType){
	case 0:
		return feof((FILE*)m_stream);
	case 1:
		return m_Position>=m_Size;
	}
	return -1;
}
//////////////////////////////////////////////////////////
long CxFile::Error()
{
	if (m_stream==NULL) return -1;
	switch(m_StorageType){
	case 0:
		return ferror((FILE*)m_stream);
	case 1:
		return m_Position>=m_Size;
	}
	return -1;
}
//////////////////////////////////////////////////////////
long CxFile::PutC(unsigned char c)
{
	if (m_stream==NULL) return -1;
	switch(m_StorageType){
	case 0:
		return fputc(c, (FILE*)m_stream);
	case 1:
		{
			if (m_Position + 1 > m_Edge) Alloc(m_Position + 1);

			memcpy((BYTE*)m_stream + m_Position, &c, 1);

			m_Position += 1;

			if (m_Position > m_Size) m_Size = m_Position;
			
			return c;
		}
	}
	return -1;
}
//////////////////////////////////////////////////////////
long CxFile::GetC()
{
	if (m_stream==NULL) return -1;
	switch(m_StorageType){
	case 0:
		return getc((FILE*)m_stream);
	case 1:
		{
		m_Position++;
		return *((BYTE*)m_stream + m_Position - 1);
		}
	}
	return -1;
}
//////////////////////////////////////////////////////////
void* CxFile::Alloc(DWORD dwNewLen)
{
	switch(m_StorageType){
	case 0:
		return m_stream;
	case 1:
		if (dwNewLen > (DWORD)m_Edge)
		{
			// determine new buffer size
			DWORD dwNewBufferSize = (DWORD)m_Edge;
			while (dwNewBufferSize < dwNewLen) dwNewBufferSize += 4096;

			// allocate new buffer
			if (m_stream == NULL) m_stream = malloc(dwNewBufferSize);
			else	m_stream = realloc(m_stream, dwNewBufferSize);

			m_Edge = dwNewBufferSize;
		}
		return m_stream;
	}
	return m_stream;
}
//////////////////////////////////////////////////////////
void CxFile::Free()
{
	Close();
}
//////////////////////////////////////////////////////////
