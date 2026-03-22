//--- K.I.LIB ---
// kl_misc.h : common-classes for K.I.LIB

#ifndef AFX_KILIBBASE_H__89998F34_A9FE_4A27_A159_671F85AA9383__INCLUDED_
#define AFX_KILIBBASE_H__89998F34_A9FE_4A27_A159_671F85AA9383__INCLUDED_

#include <cstring>

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
// ?g?p????O??????????????}?N???????????
// ??L?F?????????????????c?H?@???@??????(^^;
// ki_strcpy / ki_strcat ?????z??Q??????? strcpy_s / strcat_s ???g???i???E?t???j

#define ki_strlen(p)      ::lstrlen(p)
template<size_t N>
inline errno_t ki_strcpy( char (&dest)[N], const char* src )
{
	return ::strcpy_s( dest, N, src );
}
inline errno_t ki_strcpy( char* dest, size_t destsz, const char* src )
{
	return ::strcpy_s( dest, destsz, src );
}
template<size_t N>
inline errno_t ki_strcat( char (&dest)[N], const char* src )
{
	return ::strcat_s( dest, N, src );
}
inline errno_t ki_strcat( char* dest, size_t destsz, const char* src )
{
	return ::strcat_s( dest, destsz, src );
}
#define ki_strcmp(p,s)    ::lstrcmp(p,s)
#define ki_strcmpi(p,s)   ::lstrcmpi(p,s)
#define ki_memzero(p,l)   ::ZeroMemory(p,l)
#define ki_memcpy(p,s,l)  ::CopyMemory(p,s,l)
#define ki_memmov(p,s,l)  ::MoveMemory(p,s,l)
#define ki_memset(p,c,l)  ::FillMemory(p,l,c)
inline bool ki_memcmp( const char* x, const char* y, int l )
{
	while( *x++==*y++ && --l );
	return l==0;
}

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/
// kiArray : ?z??????L???[??????

template <class T> class kiArray
{
public: //-- ?O?????C???^?[?t?F?C?X --------------------------

	// ?????T?C?Y( 10?`255 )???????
	kiArray( unsigned char start_size=10 )
		{
			m_pArray = new T[ m_ALen=start_size ];
			m_Len = 0;
		}

	// ?z?????????
	void empty()
		{
			m_Len = 0;
		}

	// ?v?f?????ï
	unsigned long len() const
		{
			return m_Len;
		}

	// ??????????
	void add( const T& obj )
		{
			if( m_Len>=m_ALen )
			{
				T* p = new T[ m_ALen<<=1 ];
				for( unsigned long i=0; i!=m_Len; i++ )
					p[i] = m_pArray[i];
				delete [] m_pArray;
				m_pArray = p;
			}
			m_pArray[ m_Len++ ] = obj;
		}

	// ?v?f??A?N?Z?X
	T& operator []( unsigned long i )
		{
			return m_pArray[i];
		}
	const T& operator []( unsigned long i ) const
		{
			return m_pArray[i];
		}

	// ?z??R?s?[
	kiArray<T>& operator = ( const kiArray<T>& o )
		{
			if( &o != this )
			{
				empty();
				for( unsigned int i=0; i!=o.len(); i++ )
					add( o[i] );
			}
			return *this;
		}

	//-- ????????????\?b?h?Q
	void alloc( unsigned long x )
		{
			if( x > m_ALen )
			{
				T* p = new T[ m_ALen=(m_ALen<<1)>x?(m_ALen<<1):x ];
				for( unsigned long i=0; i!=m_Len; i++ )
					p[i] = m_pArray[i];
				delete [] m_pArray;
				m_pArray = p;
			}
		}
	void forcelen( unsigned long x )
		{
			alloc( x );
			m_Len = x;
		}

private: //-- ???????? -----------------------------------

	T* m_pArray;
	unsigned long m_Len, m_ALen;

public:

	virtual ~kiArray()
		{
			delete [] m_pArray;
		}
};

#define  BoolArray kiArray<bool>
#define  CharArray kiArray<char*>
#define cCharArray kiArray<const char*>
#define   StrArray kiArray<kiStr>

#endif
