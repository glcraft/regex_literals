#pragma once
#ifndef REGEX_LITERAL_FLAGS
#define REGEX_LITERAL_FLAGS 1
#endif
#include <regex>
#include <string>
// Match result with string inside
template <typename CharT>
struct match_results_ws: public std::match_results<typename std::basic_string<CharT>::const_iterator>
{
public:
	using _string = std::basic_string<CharT>;
	match_results_ws() : std::match_results<typename _string::const_iterator>() {}
	match_results_ws(_string _s) : std::match_results<typename _string::const_iterator>(), s(_s) {}
	_string& str()
	{return s;}
	const _string& str() const
	{return s;}
protected:
	_string s;
};
template <typename CharT>
struct struct_rm : public std::basic_regex<CharT>
{
    using _string = std::basic_string<CharT>;
    typedef typename _string::const_iterator const_iterator; 
    typedef std::match_results<const CharT *> _cmatch;
	typedef std::match_results<const_iterator > _smatch;
	typedef match_results_ws<CharT> _smatch_ws;
	
    struct_rm(_string str)
    {
#if REGEX_LITERAL_FLAGS
		std::regex_constants::syntax_option_type flags;
		if (check_regex_flags(str, flags))
			this->assign(str, flags);
		else
#endif
        this->assign(str);
    }
    struct_rm(const CharT *_Str, size_t _Len) : struct_rm(_string(_Str, _Len))
    {
		
    }

	// Deleted method
	// To get the match results, we have to get the string reference !
	// Use match_results_ws instead to get the string into it
	inline bool match(_string&& str, _smatch& sm) = delete;
	// Deleted method
	// To get the match results, we have to get the string reference !
	// Use match_results_ws instead to get the string into it.
	inline bool search(_string&& str, _smatch& sm) = delete;

	
	inline bool match(_string&& str, _smatch_ws& sm)
	{
		sm.str().swap(str);
		return std::regex_match(sm.str(), sm, *this);
	}
	inline bool search(_string&& str, _smatch_ws& sm)
	{
		sm.str().swap(str);
		return std::regex_search(sm.str(), sm, *this);
	}
	inline bool match(const CharT* str, _cmatch& sm)
	{
		return std::regex_match(str, sm, *this);
	}
    inline bool match(const _string& str, _smatch& sm)
    {
        return std::regex_match(str, sm, *this);
    }
	inline bool match(const CharT* str)
	{
		return std::regex_match(str, *this);
	}
	inline bool match(const _string& str)
	{
		return std::regex_match(str, *this);
	}
    inline bool match(_string&& str)
    {
        return std::regex_match(str, *this);
    }
	inline bool search(const CharT* str, _cmatch& sm)
	{
		return std::regex_search(str, sm, *this);
	}
    inline bool search(const _string& str, _smatch& sm)
    {
        return std::regex_search(str, sm, *this);
    }
	inline bool search(const CharT* str)
	{
		return std::regex_search(str, *this);
	}
	inline bool search(const _string& str)
	{
		return std::regex_search(str, *this);
	}
    inline bool search(_string&& str)
    {
        return std::regex_search(str, *this);
    }
private:
#if REGEX_LITERAL_FLAGS
	static CharT const *RegexFlagsDetector;
	bool check_regex_flags(_string& _str, std::regex_constants::syntax_option_type& _flags)
	{
		using namespace std;
		using namespace regex_constants;
		static const std::basic_regex<CharT> reg(RegexFlagsDetector);
		_smatch sm;
		syntax_option_type flags = static_cast<syntax_option_type>(0);
		syntax_option_type engine = ECMAScript;
		if (regex_match(_str,sm,reg))
		{
			auto& beg = sm[2].first, end = sm[2].second;
			for (auto itL=beg; itL != end; itL++)
			{
				CharT l = *itL;
				switch (l)
				{
				case 'i': flags |= icase; break;
				case 'b': flags |= nosubs; break;
				case 'o': flags |= optimize; break;
				case 'c': flags |= regex_constants::collate; break;
#if __cplusplus >= 201703L
				case 'm': flags |= multiline; break;
#endif
				case 'E': engine = ECMAScript; break;
				case 'B': engine = basic; break;
				case 'X': engine = extended; break;
				case 'A': engine = awk; break;
				case 'G': engine = grep; break;
				case 'P': engine = egrep; break;
				}
			}
			_str = sm[1];
			_flags = flags | engine;
			return true;
		}
		return false;
	}
	std::regex_constants::syntax_option_type check_regex_flags(const CharT *_Str, size_t _Len)
	{
		return check_regex_flags(_string(_Str, _Len));
	}
#endif
};

inline namespace literals
{
    inline namespace regex_literals
    {
        inline struct_rm<char> operator "" _rg(const char *_Str, size_t _Len)
        {	// construct literal from [_Str, _Str + _Len)
            return (struct_rm<char>(_Str, _Len));
        }
		inline struct_rm<wchar_t> operator "" _rg(const wchar_t *_Str, size_t _Len)
        {	// construct literal from [_Str, _Str + _Len)
            return (struct_rm<wchar_t>(_Str, _Len));
        }
    }
}
#if REGEX_LITERAL_FLAGS
template <>
const char* struct_rm<char>::RegexFlagsDetector = R"(^\/(.*)\/([ibocmEXBAGP]*)$)";

template <>
const wchar_t* struct_rm<wchar_t>::RegexFlagsDetector = LR"(^\/(.*)\/([ibocmEXBAGP]*)$)";
#endif