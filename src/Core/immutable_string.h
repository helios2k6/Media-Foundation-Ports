// Copyright (c) 2013 Craig Henderson
// https://github.com/cdmh/cpp_immutable_string
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <memory>
#include <string>

// "MSVC2013 Preview" didn't have initializer_list but _MSC_VER was defined 1800,
// so if you are using that compiler, you'll need to modify this condition
#if !defined(_MSC_VER)  ||  _MSC_VER > 1700
#define HAS_INITIALIZER_LIST 1
#include <initializer_list>
#endif

#ifdef __GNUC__
#define GCC_VERSION (__GNUC__ * 10000 \
                   + __GNUC_MINOR__ * 100 \
                   + __GNUC_PATCHLEVEL__)
#if GCC_VERSION <= 40801
#define _LIBSTDC_BUG_53221_WORKAROUND
#endif
#endif

namespace cdmh {

#if !defined(_MSC_VER)  ||  _MSC_VER <= 1800
#define noexcept throw()
#endif

template<typename Char,
         typename Traits = std::char_traits<Char>,    // basic_string::traits_type
         typename Alloc = std::allocator<Char>>       // basic_string::allocator_type
class basic_immutable_string
{
  public:
    typedef Traits traits_type;
    typedef Alloc  allocator_type;
    typedef Char  value_type;
    typedef typename Alloc::const_reference        const_reference;
    typedef typename Alloc::const_pointer          const_pointer;
    typedef typename Alloc::difference_type        difference_type;
    typedef typename Alloc::size_type              size_type;
    typedef typename std::basic_string<Char, Traits, Alloc>::const_iterator         const_iterator;
    typedef typename std::basic_string<Char, Traits, Alloc>::const_reverse_iterator const_reverse_iterator;

    static size_type const npos = (size_type)-1;

/* we are not defining the mutable types:
    typedef typename std::basic_string<Char, Traits, Alloc>::reference              reference;
    typedef typename std::basic_string<Char, Traits, Alloc>::pointer                pointer;
    typedef typename std::basic_string<Char, Traits, Alloc>::iterator               iterator;
    typedef typename std::basic_string<Char, Traits, Alloc>::reverse_iterator       reverse_iterator;
*/

    /*
      constructors
    */
    // default
    explicit basic_immutable_string(allocator_type const &alloc = allocator_type()) : string_(alloc)                           { }

    // copy
    basic_immutable_string(basic_immutable_string const &str) : string_(str.string_)                                           { }
#ifndef _LIBSTDC_BUG_53221_WORKAROUND
    basic_immutable_string(basic_immutable_string const &str, allocator_type const &alloc) : string_(str.string_, alloc)       { }
#endif

    // substring
    basic_immutable_string(basic_immutable_string const &str, size_type pos, size_type len = npos,
                           allocator_type const &alloc = allocator_type()) : string_(str.string_, pos, len, alloc)             { }

    // from c-string
    basic_immutable_string(Char const * const s, allocator_type const &alloc = allocator_type()) : string_(s, alloc)          { }

    // from buffer
    basic_immutable_string(Char const * const s, size_type n,
                           allocator_type const &alloc = allocator_type()) : string_(s, n, alloc)                              { }

    // fill
    basic_immutable_string(size_type n, Char c,
                           allocator_type const &alloc = allocator_type()) : string_(n, c, alloc)                              { }

    basic_immutable_string(Char c, allocator_type const &alloc = allocator_type()) : string_(1, c, alloc)                     { }

    // range
    template<typename InputIterator>
    basic_immutable_string(InputIterator first, InputIterator last,
                           allocator_type const &alloc = allocator_type()) : string_(first, last, alloc)                       { }
#if HAS_INITIALIZER_LIST
    // initializer list
    basic_immutable_string(std::initializer_list<Char> il,
                           allocator_type const &alloc = allocator_type()) : string_(il, alloc)                                { }
#endif

    // move
    basic_immutable_string(basic_immutable_string &&str) noexcept : string_(std::move(str.string_))                            { }
#ifndef _LIBSTDC_BUG_53221_WORKAROUND
    basic_immutable_string(basic_immutable_string &&str, allocator_type const &alloc) : string_(std::move(str.string_), alloc) { }
#endif

    // custom ctors (i.e. not from the C++ std::basic_string
    basic_immutable_string(std::basic_string<Char, Traits, Alloc> const &str) : string_(str)                                  { }
    basic_immutable_string(std::basic_string<Char, Traits, Alloc> &&str) : string_(std::forward<std::basic_string<Char, Traits, Alloc>>(str)) { }

    int const compare(basic_immutable_string const &str)                                                     const noexcept { return string_.compare(str.string_);                       }
    int const compare(std::basic_string<Char, Traits, Alloc> const &str)                                     const noexcept { return string_.compare(str);                               }
    int const compare(size_type pos, size_type len, basic_immutable_string const &str)                       const          { return string_.compare(pos,len,str.string_);               }
    int const compare(size_type pos, size_type len,                                                          
                      std::basic_string<Char, Traits, Alloc> const &str)                                     const          { return string_.compare(pos,len,str);                       }
    int const compare(size_type pos, size_type len, basic_immutable_string const &str,                       
                      size_type subpos, size_type sublen)                                                    const          { return string_.compare(pos,len,str.string_,subpos,sublen); }
    int const compare(size_type pos, size_type len, std::basic_string<Char, Traits, Alloc> const &str,        
                      size_type subpos, size_type sublen)                                                    const          { return string_.compare(pos,len,str,subpos,sublen);         }
    int const compare(Char const *s)                                                                         const          { return string_.compare(s);                                 }
    int const compare(size_type pos, size_type len, Char const *s)                                           const          { return string_.compare(pos,len,s);                         }
    int const compare(size_type pos, size_type len, Char const *s, size_type n)                              const          { return string_.compare(pos,len,s,n);                       }
                                                                                                             
    // Iterators                                                                                             
    const_iterator         cbegin(void)                                                                      const noexcept { return string_.cbegin();                                   }
    const_iterator         cend(void)                                                                        const noexcept { return string_.cend();                                     }
    const_reverse_iterator crbegin(void)                                                                     const noexcept { return string_.crbegin();                                  }
    const_reverse_iterator crend(void)                                                                       const noexcept { return string_.crend();                                    }
                                                                                                                                                                                         
    // Capacity                                                                                                                                                                          
    bool             const empty(void)                                                                       const noexcept { return string_.empty();                                    }
    size_type        const length(void)                                                                      const noexcept { return string_.length();                                   }
    size_type        const size(void)                                                                        const noexcept { return string_.size();                                     }
    size_type        const max_size(void)                                                                    const noexcept { return string_.max_size();                                 }
    size_type        const capacity(void)                                                                    const noexcept { return string_.capacity();                                 }
                                                                                                                                                                                         
    // Element access                                                                                                                                                                    
    const_reference         operator[](size_type pos)                                                        const          { return string_[pos];                                       }
    const_reference         at(size_type pos)                                                                const          { return string_.at(pos);                                    }
    Char            const &back(void)                                                                        const          { return string_.back();                                     }
    Char            const &front(void)                                                                       const          { return string_.front();                                    }
                                                                                                             
    /*                                                                                                       
      Modifiers                                                                                              
    */                                                                                                       
    basic_immutable_string substr(size_type pos=0, size_type len=npos)                                       const          { return basic_immutable_string(*this, pos, len);            }

    basic_immutable_string append(basic_immutable_string const &str)                                         const;    // immutable string
    basic_immutable_string append(std::basic_string<Char, Traits, Alloc> const &str)                         const;    // string
    basic_immutable_string append(basic_immutable_string const &str,                                         
                                  size_type subpos, size_type sublen)                                        const;    // substring (immutable source)
    basic_immutable_string append(std::basic_string<Char, Traits, Alloc> const &str,                        
                                  size_type subpos, size_type sublen)                                        const;    // substring
    basic_immutable_string append(Char const * const s)                                                      const;    // c-string
    basic_immutable_string append(Char const * const s, size_type n)                                         const;    // buffer
    basic_immutable_string append(size_type n, Char c)                                                       const;    // fill
    basic_immutable_string append(Char c)                                                                    const;
    template<typename InputIterator>                                                                           
    basic_immutable_string append(InputIterator first, InputIterator last)                                   const;    // range
#if HAS_INITIALIZER_LIST                                                                                     
    basic_immutable_string append(std::initializer_list<Char> il)                                            const;    // initializer list
#endif                                                                                                       
                                                                                                             
    basic_immutable_string insert(size_type pos, basic_immutable_string const &str)                          const;    // immutable string
    basic_immutable_string insert(size_type pos, std::basic_string<Char, Traits, Alloc> const &str)          const;    // string
    basic_immutable_string insert(size_type pos, basic_immutable_string const &str,                                    // substring (immutable source)
                                  size_type subpos, size_type sublen)                                        const;    // substring
    basic_immutable_string insert(size_type pos, std::basic_string<Char, Traits, Alloc> const &str,         
                                  size_type subpos, size_type sublen)                                        const;    // substring
    basic_immutable_string insert(size_type pos, Char const *s)                                              const;    // c-string
    basic_immutable_string insert(size_type pos, Char const *s, size_type n)                                 const;    // buffer
    basic_immutable_string insert(size_type pos,   size_type n, Char c)                                      const;    // fill
    basic_immutable_string insert(const_iterator p, size_type n, Char c)                                     const;    // fill
    basic_immutable_string insert(const_iterator p, Char c)                                                  const;    // single character
    template<typename InputIterator>                                                                           
    basic_immutable_string insert(const_iterator p, InputIterator first, InputIterator last)                 const;    // range
#if HAS_INITIALIZER_LIST                                                                                     
    basic_immutable_string insert(const_iterator p, std::initializer_list<Char> il)                          const;    // initializer list
#endif                                                                                                       
                                                                                                             
    basic_immutable_string erase(size_type pos=0, size_type len=npos)                                        const;    // sequence
    basic_immutable_string erase(const_iterator p)                                                           const;    // character
    basic_immutable_string erase(const_iterator first, const_iterator last)                                  const;    // range
                                                                                                             
                                                                                                             
    basic_immutable_string replace(size_type pos, size_type len, basic_immutable_string const &str)          const;    // string
    basic_immutable_string replace(size_type pos, size_type len,                                             
                                   std::basic_string<Char, Traits, Alloc> const &str)                        const;    // string
    basic_immutable_string replace(const_iterator i1, const_iterator i2,                                     
                                   basic_immutable_string const &str)                                        const;    // string
    basic_immutable_string replace(const_iterator i1, const_iterator i2,                                     
                                   std::basic_string<Char, Traits, Alloc> const &str)                        const;    // string
                                                                                                             
    basic_immutable_string replace(size_type pos, size_type len,                                             
                                   basic_immutable_string const &str,                                        
                                   size_type subpos, size_type sublen)                                       const;    // substring
    basic_immutable_string replace(size_type pos, size_type len,                                             
                                   std::basic_string<Char, Traits, Alloc> const &str,                       
                                   size_type subpos, size_type sublen)                                       const;    // substring
                                                                                                             
    basic_immutable_string replace(size_type pos,     size_type len,     Char const *s)                      const;    // c-string
    basic_immutable_string replace(const_iterator i1, const_iterator i2, Char const *s)                      const;    // c-string
                                                                                                              
    basic_immutable_string replace(size_type pos,     size_type len,     Char const *s, size_type n)         const;    // buffer
    basic_immutable_string replace(const_iterator i1, const_iterator i2, Char const *s, size_type n)         const;    // buffer
                                                                                                              
    basic_immutable_string replace(size_type pos,     size_type len,     size_type n, Char c)                const;    // fill
    basic_immutable_string replace(const_iterator i1, const_iterator i2, size_type n, Char c)                const;    // fill
                                                                                                             
    template<typename InputIterator>                                                                           
    basic_immutable_string replace(const_iterator i1, const_iterator i2,                                     
                                   InputIterator first, InputIterator last)                                  const;    // range
                                                                                                             
#if HAS_INITIALIZER_LIST                                                                                     
    basic_immutable_string replace(const_iterator i1, const_iterator i2,                                     
                                   std::initializer_list<Char> il)                                           const;    // initializer list
#endif                                                                                                       
                                                                                                             
    Char const *                     const c_str(void)                                                       const noexcept { return string_.c_str();         }
    Char const *                     const data(void)                                                        const noexcept { return string_.data();          }
    std::basic_string<Char, Traits, Alloc> mutable_string(void)                                              const          { return string_;                 }
    allocator_type                         get_allocator(void)                                               const noexcept { return string_.get_allocator(); }
    size_type                        const copy(Char* s, size_type len, size_type pos)                       const          { return string_.copy(s,len,pos); }

    size_type const find(basic_immutable_string const &str, size_type pos=0)                                 const noexcept;    // string
    size_type const find(std::basic_string<Char, Traits, Alloc> const &str, size_type pos=0)                 const noexcept;    // string
    size_type const find(Char const *s, size_type pos=0)                                                     const;             // c-string
    size_type const find(Char const *s, size_type pos, size_type n)                                          const;             // buffer
    size_type const find(Char c, size_type pos=0)                                                            const noexcept;    // character
                                                                                                             
    size_type const rfind(basic_immutable_string const &str, size_type pos=npos)                             const;             // string
    size_type const rfind(std::basic_string<Char, Traits, Alloc> const &str, size_type pos=npos)             const;             // string
    size_type const rfind(Char const *s, size_type pos=npos)                                                 const;             // c-string
    size_type const rfind(Char const *s, size_type pos, size_type n)                                         const;             // buffer
    size_type const rfind(Char c, size_type pos=npos)                                                        const;             // character
                                                                                                             
    size_type const find_first_of(basic_immutable_string const &str, size_type pos=0)                        const noexcept;    // string
    size_type const find_first_of(std::basic_string<Char, Traits, Alloc> const &str, size_type pos=0)        const noexcept;    // string
    size_type const find_first_of(Char const *s, size_type pos=0)                                            const;             // c-string
    size_type const find_first_of(Char const *s, size_type pos, size_type n)                                 const;             // buffer
    size_type const find_first_of(Char c, size_type pos=0)                                                   const noexcept;    // character
                                                                                                             
    size_type const find_last_of(basic_immutable_string const &str, size_type pos=npos)                      const noexcept;    // string
    size_type const find_last_of(std::basic_string<Char, Traits, Alloc> const &str, size_type pos=npos)      const noexcept;    // string
    size_type const find_last_of(Char const *s, size_type pos=npos)                                          const;             // c-string
    size_type const find_last_of(Char const *s, size_type pos, size_type n)                                  const;             // buffer
    size_type const find_last_of(Char c, size_type pos=npos)                                                 const noexcept;    // character
                                                                                                             
    size_type const find_first_not_of(basic_immutable_string const &str, size_type pos=0)                    const noexcept;    // string
    size_type const find_first_not_of(std::basic_string<Char, Traits, Alloc> const &str, size_type pos=0)    const noexcept;    // string
    size_type const find_first_not_of(Char const *s, size_type pos=0)                                        const;             // c-string
    size_type const find_first_not_of(Char const *s, size_type pos, size_type n)                             const;             // buffer
    size_type const find_first_not_of(Char c, size_type pos=0)                                               const noexcept;    // character

    size_type const find_last_not_of(basic_immutable_string const &str, size_type pos=npos)                  const noexcept;    // string
    size_type const find_last_not_of(std::basic_string<Char, Traits, Alloc> const &str, size_type pos=npos)  const noexcept;    // string
    size_type const find_last_not_of(Char const *s, size_type pos=npos)                                      const;             // c-string
    size_type const find_last_not_of(Char const *s, size_type pos, size_type n)                              const;             // buffer
    size_type const find_last_not_of(Char c, size_type pos=npos)                                             const noexcept;    // character

  private:
    // the encapsulated string is not declared const as this would
    // prevent the object being moved, which may be important in
    // some situations for performance
    std::basic_string<Char, Traits, Alloc> string_;

#if defined(_MSC_VER)  &&  _MSC_VER < 1800
    // private assignment operator prevents compiler generator function
    basic_immutable_string &operator=(basic_immutable_string);
#else
    basic_immutable_string &operator=(basic_immutable_string) = delete;
#endif
};

typedef basic_immutable_string<char>     immutable_string;
typedef basic_immutable_string<wchar_t>  immutable_wstring;
typedef basic_immutable_string<char16_t> immutable_u16string;
typedef basic_immutable_string<char32_t> immutable_u32string;

}   // namespace cdmh

#include "immutable_string.inl"
