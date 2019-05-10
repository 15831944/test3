#ifndef __STRING_H__
#define __STRING_H__

namespace Leaf
{
	namespace Text
	{
		template<typename T>
		class LString /*: public Object*/
		{
		public:
			LString();
 			LString(const T& _char);
			LString(const T* _buffer, v_int _length);
 			LString(const T* _buffer, bool copy = true);
 			LString(const LString<T>& string);

			~LString();
			
		public:
			LString<T>& operator=(const T& _char);
			LString<T>& operator=(const T* _buffer);
			LString<T>& operator=(const LString<T>& string);

			LString<T>& operator+=(const T& _char);
			LString<T>& operator+=(const T* _buffer);
			LString<T>& operator+=(const LString<T>& string);

			LString<T> operator+(const T& _char);
			LString<T> operator+(const T* _buffer);
			LString<T> operator+(const LString<T>& string);
			
			bool operator==(const LString<T>& string)const;
			bool operator!=(const LString<T>& string)const;
			bool operator>(const LString<T>& string)const;
			bool operator>=(const LString<T>& string)const;
			bool operator<(const LString<T>& string)const;
			bool operator<=(const LString<T>& string)const;
			bool operator==(const T* buffer)const;
			bool operator!=(const T* buffer)const;
			bool operator>(const T* buffer)const;
			bool operator>=(const T* buffer)const;
			bool operator<(const T* buffer)const;
			bool operator<=(const T* buffer)const;
			
			T operator[](v_int index)const;

		public:
			
		public:
			v_int				Collate(const LString<T>& string)const;
			v_int				Compare(const LString<T>& string)const;
			
			v_int				GetLength()const;
//			v_int				Find()const;
//			v_int				FindOneOf()const;
			v_int 				IndexOf(T c)const;
			
			v_int				Delete(v_int index, v_int count)const;
			
			LString<T>			Left(v_int count)const;
			LString<T> 			Right(v_int count)const;
			LString<T> 			Mid(v_int index,    v_int count)const;
			LString<T> 			Remove(v_int index, v_int count)const;
			LString<T> 			Insert(v_int index, const LString<T>& string)const;
			
		protected:
			v_int				Inc()const;
			v_int				Dec()const;

			LString(const LString<T>& string,  v_int _start, v_int _length);
			LString(const LString<T>& string,  const T* _buffer, v_int _index, v_int _count);
			LString(const LString<T>& strDest, const LString<T>& strSource, v_int _index, v_int _count);
			
			static v_int		GetLength(const T* _buffer);
			static v_int		StringCompare(const T* pszA, const LString<T>& pstrString);
//			static v_int		StringCompareIgnore(const T* pszA, const LString<T>& pstrString);
//			static v_int		StringCollate(const T* pszA, const LString<T>& pstrString);
//			static v_int		StringCollateIgnore(const T* pszA, const LString<T>& pstrString);
			
			
		private:
			static const T		zero;
			mutable T*			buffer;	
			mutable volatile v_int*	counter;
			mutable v_int		start;
			mutable v_int		length;
			mutable v_int		realLength;	
		};
		
		//////////////////////////////////////////////////////////////////////////
		//		
		template<typename T>
		LString<T>::LString()
		{
			buffer  = (T*)&zero;
			counter	= 0;
			start   = 0;
			length  = 0;
			realLength = 0;	
		}
			
		//
		template<typename T>
		LString<T>::LString(const T& _char)
		{
			buffer  = new T[2];
			counter = new v_int(1); 
			start   = 0;
			length  = 1;
			realLength = 1;
			
			buffer[0] = _char;
			buffer[1] = 0;
		}

		//
		template<typename T>
		LString<T>::LString(const T* _buffer, v_int _length)
		{
			v_int nlen = _length;

			if(nlen <= 0)
			{
				buffer  = (T*)&zero;
				counter	= 0;
				start   = 0;
				length  = 0;
				realLength = 0;
			}	
			else
			{
				buffer  = new T[nlen+1];
				counter = new v_int(1);
				start   = 0;
				length  = nlen;
				realLength = nlen;
				
				memcpy(buffer, _buffer, nlen*sizeof(T));
				buffer[nlen] = 0;
			}
		}

		//
		template<typename T>
		LString<T>::LString(const T* _buffer, bool copy)
		{
			v_int nlen = GetLength(_buffer);

			if(copy)
			{
				length  = nlen;
				buffer  = new T[nlen+1];
				counter=new v_int(1);
				start=0;
				realLength=nlen;
				
				memcpy(buffer, _buffer, nlen*sizeof(T));
				buffer[nlen] = 0;
			}
			else
			{
				buffer  = (T*)_buffer;
				counter = 0;
				start   = 0;
				length	= nlen;
				realLength = nlen;
			}		
		}
		
		//
		template<typename T>
		LString<T>::LString(const LString<T>& string)
		{
			buffer  = string.buffer;
			counter = string.counter;
			start   = string.start;
			length  = string.length;
			realLength = string.realLength;

			Inc();
		}

		//
		template<typename T>
		LString<T>::~LString()
		{
			Dec();
		}
		
		//////////////////////////////////////////////////////////////////////////
		//
		template<typename T>
		LString<T>& LString<T>::operator=(const T& _char)
		{
			T _buffer[2] = {0};
			_buffer[0] = _char;
			_buffer[1] = 0;

			v_int buflen = GetLength(_buffer);
			if (buflen <= 0)
			{
				buffer  = (T*)&zero;
				counter = 0;
				start   = 0;
				length  = 0;
				realLength = 0;

				return *this;
			}

			if (Dec() == 0)
			{
				buffer  = new T[buflen+1];
				counter = new v_int(1);
				start   = 0;
				length  = buflen;
				realLength = buflen;

				memcpy(buffer, _buffer, sizeof(T)*buflen);
				buffer[buflen] = 0;
			}

			return *this;
		}

		//
		template<typename T>
		LString<T>& LString<T>::operator=(const T* _buffer)
		{
			v_int buflen = GetLength(_buffer);
			
			if (buflen <= 0)
			{
				buffer  = (T*)&zero;
				counter = 0;
				start   = 0;
				length  = 0;
				realLength = 0;

				return *this;
			}

			if (Dec() == 0)
			{
				buffer  = new T[buflen+1];
				counter = new v_int(1);
				start   = 0;
				length  = buflen;
				realLength = buflen;

				memcpy(buffer, _buffer, sizeof(T)*buflen);
				buffer[buflen] = 0;
			}

			return *this;
		}

		//
		template<typename T>
		LString<T>& LString<T>::operator=(const LString<T>& string)
		{
			if (this == &string)
			{
				return *this;
			}

			if (Dec() == 0)
			{
				buffer  = string.buffer;
				counter = string.counter;
				start   = string.start;
				length  = string.length;
				realLength = string.realLength;
			}

			Inc();
			return *this;
		}

		//
		template<typename T>
		LString<T>& LString<T>::operator+=(const T& _char)
		{
			T _buffer[2] = {0};
			_buffer[0] = _char;
			_buffer[1] = 0;

			return *this = *this + _buffer;
		}

		//
		template<typename T>
		LString<T>& LString<T>::operator+=(const T* _buffer)
		{
			return *this = *this + _buffer;
		}

		//
		template<typename T>
		LString<T>& LString<T>::operator+=(const LString<T>& string)
		{
			return *this = *this + string;
		}
	
		//
		template<typename T>
		LString<T> LString<T>::operator +(const T& _char)
		{
			T _buffer[2] ={0};
			_buffer[0] = _char;
			_buffer[1] = 0;

			return LString<T>(*this, _buffer, length, 0);
		}

		//
		template<typename T>
		LString<T> LString<T>::operator+(const T* _buffer)
		{
			return LString<T>(*this, _buffer, length, 0);
		}

		//
		template<typename T>
		LString<T> LString<T>::operator+(const LString<T>& string)
		{
			return LString<T>(*this, string, length, 0);		//Concatenate
		}
		
		//
		template<typename T>
		bool LString<T>::operator==(const LString<T>& string)const
		{
			return false;
		}
		
		template<typename T>
		bool LString<T>::operator!=(const LString<T>& string)const
		{
			return false;	
		}
		
		template<typename T>
		bool LString<T>::operator>(const LString<T>& string)const
		{
			return false;
		}
		
		template<typename T>
		bool LString<T>::operator>=(const LString<T>& string)const
		{
			return false;
		}
		
		template<typename T>
		bool LString<T>::operator<(const LString<T>& string)const
		{
			return false;
		}
		
		template<typename T>
		bool LString<T>::operator<=(const LString<T>& string)const
		{
			return false;
		}
		
		template<typename T>
		bool LString<T>::operator==(const T* buffer)const
		{
			return false;
		}
		
		template<typename T>
		bool LString<T>::operator!=(const T* buffer)const
		{
			return false;
		}
		
		template<typename T>
		bool LString<T>::operator>(const T* buffer)const
		{
			return false;
		}
		
		template<typename T>
		bool LString<T>::operator>=(const T* buffer)const
		{
			return false;
		}
		
		template<typename T>
		bool LString<T>::operator<(const T* buffer)const
		{
			return false;
		}
		
		template<typename T>
		bool LString<T>::operator<=(const T* buffer)const
		{
			return false;
		}
		
		template<typename T>
		T LString<T>::operator[](v_int index)const
		{
			return buffer[start+index];
		}
		//////////////////////////////////////////////////////////////////////////
		//
		template<typename T>
		v_int LString<T>::Inc()const
		{
			if(counter)
			{
				INCRC(counter);
			}	

			return *counter;
		}
		
		template<typename T>
		v_int LString<T>::Dec()const
		{
			if (counter == 0)
			{
				return 0;
			}

			if (DECRC(counter)==0)
			{
				delete[] buffer;
				buffer = (T*)&zero;

				delete counter;
				counter = 0;

				return 0;
			}

			return *counter;
		}

		template<typename T>
		LString<T>::LString(const LString<T>& string, v_int _start, v_int _length)
		{
// 			if (_length)
// 			{
// 				buffer=(T*)&zero;
// 				counter=0;
// 				start=0;
// 				length=0;
// 				realLength=0;
// 			}
// 			else
// 			{
// 				buffer=string.buffer;
// 				counter=string.counter;
// 				start=string.start+_start;
// 				length=_length;
// 				realLength=string.realLength;
// 
// 				Inc();
// 			}
		}

		template<typename T>
		LString<T>::LString(const LString<T>& string, const T* _buffer, v_int _index, v_int _count)
		{
			v_int buflen = GetLength(_buffer);

			if (_index==0 && buflen==0 && _count==string.length)
			{
				buffer  = (T*)&zero;
				counter = 0;
				start   = 0;
				length  = 0;
				realLength = 0;
			}
			else
			{
				length  = string.length - _count + buflen;

				buffer  = new T[length+1];
				counter = new v_int(1);
				start   = 0;
				realLength = length;

				memcpy(buffer, string.buffer+string.start, sizeof(T)*_index);
				memcpy(buffer+_index, _buffer, sizeof(T)*buflen);

				memcpy(buffer+_index+buflen, string.buffer+string.start+_index+_count, sizeof(T)*string.length-_index-_count);
				buffer[length] = 0;
			}
		}

		template<typename T>
		LString<T>::LString(const LString<T>& strDest, const LString<T>& strSource, v_int _index, v_int _count)
		{
			if (_index==0 && _count==strDest.length && strSource.length==0)
			{
				buffer  = (T*)&zero;
				counter = 0;
				start   = 0;
				length  = 0;
				realLength = 0;
			}
			else
			{
				length  = strDest.length - _count + strSource.length;

				buffer  = new T[length+1];
				counter = new v_int(1);
				start   = 0;
				realLength = length;

				memcpy(buffer, strDest.buffer+strDest.start, sizeof(T)*_index);
				memcpy(buffer+_index, strSource.buffer+strSource.start, sizeof(T)*strSource.length);

				memcpy(buffer+_index+strSource.length, strDest.buffer+strDest.start+_index+_count, sizeof(T)*strDest.length-_index-_count);
				buffer[length] = 0;
			}
		}

		template<typename T>
		v_int LString<T>::GetLength(const T* _buffer)
		{
			v_int result = 0;
			while(*_buffer++) result++;
			return result;
		}

	
		template<typename T>
		v_int LString<T>::StringCompare(const T* pszA, const LString<T>& pstrString)
		{
			
		}
/*			
		template<typename T>
		v_int LString<T>::StringCompareIgnore()
		{
			
		}
		
		template<typename T>
		v_int LString<T>::StringCollate()
		{
			
		}
		
		template<typename T>
		v_int LString<T>::StringCollateIgnore()
		{
			
		}
*/		
		//////////////////////////////////////////////////////////////////////////
		//
		template<typename T>
		v_int LString<T>::Collate(const LString<T>& string)const
		{
			return 0;
		}
		
		template<typename T>
		v_int LString<T>::Compare(const LString<T>& string)const
		{
			return 0;	
		}
		
		template<typename T>
		v_int LString<T>::GetLength()const
		{
			return length;
		}
		
/*
		template<typename T>
		v_int LString<T>::Find()const
		{
			return 0;
		}


		template<typename T>
		v_int LString<T>::FindOneOf()const
		{
			return 0;
		}
*/	

		template<typename T>
		v_int LString<T>::IndexOf(T c)const
		{
			const T* read = buffer+start;
			for(v_int i=0; i<length; i++)
			{
				if(read[i] == c)
				{
					return  i;
				}
			}
			
			return -1;	
		}
		
		template<typename T>
		v_int LString<T>::Delete(v_int index, v_int count)const
		{
			return 0;
		}
		
		template<typename T>
		LString<T> LString<T>::Left(v_int count)const
		{
//			return LString<T>(*this, 0, count);
			return *this = *this + string;
		}
		
		template<typename T>
		LString<T> LString<T>::Right(v_int count)const
		{
			return *this = *this + string;
		}
		
		template<typename T>
		LString<T> LString<T>::Mid(v_int index,    v_int count)const
		{
			return *this = *this + string;
		}
		
		template<typename T>
		LString<T> LString<T>::Remove(v_int index, v_int count)const
		{
			return *this = *this + string;
		}
		
		template<typename T>
		LString<T> LString<T>::Insert(v_int index, const LString<T>& string)const
		{
			return *this = *this + string;
		}
		
		//////////////////////////////////////////////////////////////////////////
		//
		template<typename T>
		const T LString<T>::zero = 0;
		
		typedef LString<char>		AString;
		typedef LString<wchar_t>	WString;
		
		extern	v_int				atoi_t();
		extern  v_int 				wtoi_t();
		extern  v_int64_t			atoi64_t();
		extern  v_int64_t			wtoi64_t();
		extern  v_uint				atou_t();
		extern  v_uint				wtou_t();
		extern  v_uint64_t			atou64_t();
		extern  v_uint64_t			wtou64_t();
		extern 	double				atof_t();	
		extern 	double				wtof_t();
		
		extern  v_int				atoi();
		extern  v_int				wtoi();
		extern  v_int64_t			atoi64();
		extern  v_int64_t			wtoi64();	
		extern  v_uint				atou();
		extern	v_uint				wtou();
		extern  v_uint64_t			atou64();
		extern	v_uint64_t			wtou64();
		extern  double				atof();	
		extern  double				wtof();
		
		extern	AString				itoa();
		extern  WString				itow();
		extern  AString				i64toa();
		extern  WString				i64tow();
		extern	AString				utoa();
		extern	WString				utow();
		extern  AString				u64toa();
		extern  WString				u64tow();
		extern  AString				ftoa();
		extern  WString				ftow();
		
		extern	v_int				_wtoa();
		extern	AString				wtoa();
		extern	v_int				_atow();
		extern	WString				atow();
		extern	AString				aupper();
		extern	WString				wupper();
#if defined _WIN_32_
		extern	void				_itoa_s();
		extern	void				_itow_s();
		extern  void				_i64toa_s();
		extern	void				_i64tow_s();
		extern	void				_uitoa_s();
		extern	void				_uitow_s();
		extern	void				_ui64toa_s();
		extern  void				_ui64tow_s();
		extern	void				_gcvt_s();
		extern	void				_strlwr_s();
		extern  void				_strupr_s();
		extern  void				_wcslwr_s();
		extern  void				_wcsupr_s();
#endif
	}
}
#endif