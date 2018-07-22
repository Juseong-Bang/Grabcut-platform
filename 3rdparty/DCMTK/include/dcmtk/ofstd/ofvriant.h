/*
 *
 *  Copyright (C) 2016, OFFIS e.V.
 *  All rights reserved.  See COPYRIGHT file for details.
 *
 *  This software and supporting documentation were developed by
 *
 *    OFFIS e.V.
 *    R&D Division Health
 *    Escherweg 2
 *    D-26121 Oldenburg, Germany
 *
 *
 *  Module:  ofstd
 *
 *  Author:  Jan Schlamelcher
 *
 *  Purpose: Implementing tagged unions similar to C++17's std::variant.
 *
 */

#ifndef OFVARIANT_H
#define OFVARIANT_H

#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */

/** @file
 *  Declares OFvariant and related functionality.
 *  @defgroup ofvisit_variant OFvisit &ndash; OFvariant
 *  Apply a visitor to an OFvariant object.
 *  @see @ref ofget_variant "OFget" &ndash; @copybrief ofget_variant
 *  @defgroup ofget_variant OFget &ndash; OFvariant
 *  Get a pointer to the value stored in an OFvariant holding the selected alternative.
 *  @see @ref ofvisit_variant "OFvisit" &ndash; @copybrief ofvisit_variant
 */

#ifdef DCMTK_USE_CXX11_STL
#include <type_traits>
#include <cassert>
#include <limits>

template<std::size_t,typename... Alternatives>
class OFvariant_traits
{
public:
    using is_constructible = std::false_type;
    static constexpr inline std::size_t alignment() { return 1; }
    static constexpr inline std::size_t size() { return 0; }
    static void index_of();
};

template<std::size_t Index,typename Alternative0,typename... Alternatives>
class OFvariant_traits<Index,Alternative0,Alternatives...>
: OFvariant_traits<Index+1,Alternatives...>
{
public:
    using OFvariant_traits<Index+1,Alternatives...>::index_of;

    using first_alternative = Alternative0;

    static constexpr inline std::size_t alignment()
    {
        return alignof(aligned);
    }

    static constexpr inline std::size_t size()
    {
        return sizeof( first_alternative ) > OFvariant_traits<Index+1,Alternatives...>::size()
        ?
            sizeof( first_alternative )
        :
            OFvariant_traits<Index+1,Alternatives...>::size()
        ;
    }

    static std::integral_constant<std::size_t,Index> index_of( first_alternative );

private:
    struct aligned { alignas(OFvariant_traits<Index+1,Alternatives...>::alignment()) char c; first_alternative a; };
};

template<std::size_t AlternativeCount,typename... IndexAlternatives>
struct OFvariant_select_index_type {};

template<std::size_t AlternativeCount,typename IndexAlternative0,typename... IndexAlternatives>
struct OFvariant_select_index_type<AlternativeCount,IndexAlternative0,IndexAlternatives...>
{
    using type = typename std::conditional
    <
        AlternativeCount <= std::numeric_limits<IndexAlternative0>::max(),
        std::enable_if<true,IndexAlternative0>,
        OFvariant_select_index_type<AlternativeCount,IndexAlternatives...>
    >::type::type;
};

template<typename...>
class OFvariant;

template<std::size_t Index,typename Variant>
struct OFvariant_alternative;

template<std::size_t Index,typename Alternative0,typename... Alternatives>
struct OFvariant_alternative<Index,OFvariant<Alternative0,Alternatives...>>
: OFvariant_alternative<Index - 1,OFvariant<Alternatives...>> {};

template<typename Alternative0,typename... Alternatives>
struct OFvariant_alternative<0,OFvariant<Alternative0,Alternatives...>>
{
    using type = Alternative0;
};

template<typename... Alternatives>
class OFvariant
{
    using traits = OFvariant_traits<0,Alternatives...>;
    using index_type = typename OFvariant_select_index_type<sizeof...(Alternatives),std::uint8_t,std::uint16_t,std::uint32_t,std::uint64_t>::type;
    template<typename T>
    using index_of = decltype(traits::index_of(std::declval<T>()));

public:
    using variant = OFvariant;

    OFvariant()
    : m_Content()
    , m_Index( 0 )
    {
        new (m_Content) typename traits::first_alternative;
    }

    template<typename T,index_type Index = index_of<T>()>
    OFvariant( T&& t )
    : m_Content()
    , m_Index( Index )
    {
        new (m_Content) typename OFvariant_alternative<Index,OFvariant>::type( std::forward<T>( t ) );
    }

    OFvariant( OFvariant& rhs )
    : OFvariant( const_cast<const OFvariant&>( rhs ) )
    {

    }

    OFvariant( const OFvariant& rhs )
    : m_Content()
    , m_Index( rhs.index() )
    {
        copy_construct( rhs );
    }

    OFvariant( OFvariant&& rhs )
    : m_Content()
    , m_Index( rhs.index() )
    {
        move_construct( std::move( rhs ) );
    }

    template<typename T,index_type Index = index_of<T>()>
    OFvariant& operator=( T&& t )
    {
        if( m_Index != Index )
        {
            destroy();
            m_Index = Index;
            new (m_Content) typename OFvariant_alternative<Index,OFvariant>::type( std::forward<T>( t ) );
        }
        else
        {
            *reinterpret_cast<typename OFvariant_alternative<Index,OFvariant>::type*>( m_Content ) = std::forward<T>( t );
        }
        return *this;
    }

    OFvariant& operator=( OFvariant& rhs )
    {
        return *this = const_cast<const OFvariant&>( rhs );
    }

    OFvariant& operator=( const OFvariant& rhs )
    {
        if( this != &rhs )
        {
            if( m_Index != rhs.m_Index )
            {
                destroy();
                m_Index = rhs.m_Index;
                copy_construct( rhs );
            }
            else
            {
                using functor = void(OFvariant::*)(const OFvariant&);
                static const functor assignment[] =
                {
                    &OFvariant::template copy_assign_alternative<Alternatives>...
                };
                (this->*assignment[m_Index])( rhs );
            }
        }
        return *this;
    }

    OFvariant& operator=( OFvariant&& rhs )
    {
        if( this != &rhs )
        {
            if( m_Index != rhs.m_Index )
            {
                destroy();
                m_Index = rhs.m_Index;
                move_construct( std::move( rhs ) );
            }
            else
            {
                using functor = void(OFvariant::*)(OFvariant&&);
                static const functor assignment[] =
                {
                    &OFvariant::template move_assign_alternative<Alternatives>...
                };
                (this->*assignment[m_Index])( std::move( rhs ) );
            }
        }
        return *this;
    }

    ~OFvariant()
    {
        destroy();
    }

    std::size_t index() const
    {
        return m_Index;
    }

private:
    template<typename T,typename... Xs>
    friend T* OFget( OFvariant<Xs...>* );
    template<typename T,typename... Xs>
    friend const T* OFget( const OFvariant<Xs...>* );
    template<typename ReturnType,typename FN,typename... Xs>
    friend ReturnType OFvisit( FN&&, OFvariant<Xs...>& );
    template<typename ReturnType,typename FN,typename... Xs>
    friend ReturnType OFvisit( FN&&, const OFvariant<Xs...>& );

    template<typename Alternative>
    void copy_construct_alternative( const OFvariant& rhs )
    {
        new (m_Content) Alternative( *reinterpret_cast<const Alternative*>( rhs.m_Content ) );
    }

    template<typename Alternative>
    void move_construct_alternative( OFvariant&& rhs )
    {
        new (m_Content) Alternative( std::move( *reinterpret_cast<Alternative*>( rhs.m_Content ) ) );
    }

    template<typename Alternative>
    void copy_assign_alternative( const OFvariant& rhs )
    {
        *reinterpret_cast<Alternative*>( m_Content ) = *reinterpret_cast<const Alternative*>( rhs.m_Content );
    }

    template<typename Alternative>
    void move_assign_alternative( OFvariant&& rhs )
    {
        *reinterpret_cast<Alternative*>( m_Content ) = std::move( *reinterpret_cast<Alternative*>( rhs.m_Content ) );
    }

    template<typename Alternative>
    void destructor()
    {
        reinterpret_cast<Alternative*>( m_Content )->~Alternative();
    }

    template<typename ReturnType,typename Alternative,typename FN>
    ReturnType visit_alternative( FN&& fn )
    {
        return fn( *reinterpret_cast<Alternative*>( m_Content ) );
    }

    template<typename ReturnType,typename Alternative,typename FN>
    ReturnType const_visit_alternative( FN&& fn ) const
    {
        return fn( *reinterpret_cast<const Alternative*>( m_Content ) );
    }

    void copy_construct( const OFvariant& rhs )
    {
        using functor = void(OFvariant::*)(const OFvariant&);
        static const functor constructor[] =
        {
            &OFvariant::template copy_construct_alternative<Alternatives>...
        };
        assert( m_Index < sizeof...(Alternatives) );
        (this->*constructor[m_Index])( rhs );
    }

    void move_construct( OFvariant&& rhs )
    {
        using functor = void(OFvariant::*)(OFvariant&&);
        static const functor constructor[] =
        {
            &OFvariant::template move_construct_alternative<Alternatives>...
        };
        assert( m_Index < sizeof...(Alternatives) );
        (this->*constructor[m_Index])( std::move( rhs ) );
    }

    void destroy()
    {
        using functor = void(OFvariant::*)();
        static const functor destructor[] =
        {
            &OFvariant::template destructor<Alternatives>...
        };
        assert( m_Index < sizeof...(Alternatives) );
        (this->*destructor[m_Index])();
    }

    alignas(traits::alignment()) std::uint8_t m_Content[traits::size()];
    index_type m_Index;
};

template<std::size_t Index,typename X,typename T0,typename... Tn>
struct OFvariant_index_of_type
: OFvariant_index_of_type<Index + 1,X,Tn...> {};

template<std::size_t Index,typename X,typename... Tn>
struct OFvariant_index_of_type<Index,X,X,Tn...>
: std::integral_constant<std::size_t,Index> {};

template<typename T,typename... Alternatives>
constexpr bool OFholds_alternative( const OFvariant<Alternatives...>& v )
{
    return v.index() == OFvariant_index_of_type<0,T,Alternatives...>::value;
}

template<typename T,typename... Alternatives>
T* OFget( OFvariant<Alternatives...>* v )
{
    if( OFholds_alternative<T>( *v ) )
        return reinterpret_cast<T*>( v->m_Content );
    return nullptr;
}

template<typename T,typename... Alternatives>
const T* OFget( const OFvariant<Alternatives...>* v )
{
    if( OFholds_alternative<T>( *v ) )
        return reinterpret_cast<const T*>( v->m_Content );
    return nullptr;
}

template<typename ReturnType,typename FN,typename... Alternatives>
ReturnType OFvisit( FN&& fn, OFvariant<Alternatives...>& v )
{
    using functor = ReturnType(OFvariant<Alternatives...>::*)(FN&&);
    static const functor visit[] =
    {
        &OFvariant<Alternatives...>::template visit_alternative<ReturnType,Alternatives,FN>...
    };
    return (v.*visit[v.index()])( std::forward<FN>( fn ) );
}

template<typename ReturnType,typename FN,typename... Alternatives>
ReturnType OFvisit( FN&& fn, const OFvariant<Alternatives...>& v )
{
    using functor = ReturnType(OFvariant<Alternatives...>::*)(FN&&) const;
    static const functor visit[] =
    {
        &OFvariant<Alternatives...>::template const_visit_alternative<ReturnType,Alternatives,FN>...
    };
    return (v.*visit[v.index()])( std::forward<FN>( fn ) );
}

#elif !defined(DOXYGEN) // fallback implementation

// Include the actual implementation (that emulates variadic templates)
#include "dcmtk/ofstd/variadic/variant.h"

#else // NOT C++11 && NOT DOXYGEN

/** A class template that represents a type-safe union.
 *  <b><em>#include</em> "dcmtk/ofstd/ofvriant.h"</b> for using this class<br>
 *  @headerfile ofvriant.h "dcmtk/ofstd/ofvriant.h"
 *  @tparam Alternatives a set of types that may be stored in this variant. All types must be (possibly
 *    cv-qualified) object types.
 *  @details
 *  OFvariant is a custom implementation of a subset of C++17's std::variant,
 *  see http://en.cppreference.com/w/cpp/utility/variant for a description of std::variant.
 *  An instance of OFvariant at any given time holds a value of one of its alternative types.
 *  As with unions, if a variant holds a value of some object type T, the object representation of T is
 *  allocated directly within the object representation of the variant itself if possible.
 *  @note If no suitable alignment specifiers were available for the target platform, OFvariant will
 *    use a fallback implementation that stores the alternative on the heap &ndash; as opposite to std::variant.
 *  @details
 *  The preferred way to access an OFvariant object is visitation utilizing @ref ofvisit_variant "OFvisit".
 *  If a certain alternative is expected to be held by the variant, @ref ofget_variant "OFget" may be used to
 *  access it directly.
 *  @see @ref ofvisit_variant "OFvisit" &ndash; @copybrief ofvisit_variant
 *  @see @ref ofget_variant "OFget" &ndash; @copybrief ofget_variant
 *  @see OFmonostate &ndash; @copybrief OFmonostate
 *  @see @ref OFin_place_helpers "OFin_place" &ndash; @copydoc OFin_place_helpers_brief
 */
template<typename... Alternatives>
class OFvariant
{
public:

    /** Constructs a variant holding a default constructed value of the first alternative.
     *  @pre The first alternative must be default constructible.
     *  @see OFmonostate &ndash; @copybrief OFmonostate
     */
    OFvariant();

    /** Copy constructs a variant holding a copy of the value rhs holds.
     *  @param rhs a const reference to another object of equal type.
     *  @pre All alternatives must be copy constructible.
     */
    OFvariant( const OFvariant& rhs );

    /** Move constructs a variant by moving the value rhs holds.
     *  @param rhs an rvalue reference to another object of equal type.
     *  @pre All alternatives must be move constructible.
     *  @note This constructor is currently only available if C++11 support was enabled.
     */
    OFvariant( OFvariant&& rhs );

    /** Constructs a variant holding the alternative that most closely matches the given
     *  argument.
     *  @tparam T the type of the argument, will be deduced automatically.
     *  @param t an object of type `T` that will be converted to one of the alternatives.
     *  @precondition There must be at least one alternative that can be constructed from
     *    the given parameter `t` and there must be exactly one such alternative that
     *    takes precedence over the others.
     *  @attention t will be perfectly forwarded if C++11 support is available, i.e. the
     *    alternative may be move constructed from `t` if possible. Support for perfect
     *    forwarding is NOT available without C++11 support, therefore the alternative
     *    will be copy constructed in this case, this means: <b>the selected alternative
     *    must be copy constructible if pre C++11 compilers shall be supported</b>.
     *  @details
     *  <h3>Usage Example:</h3>
     *  @code{.cpp}
     *  OFvariant<int,float,long>( 3 );      // OK
     *  OFvariant<int,int>( 3 );             // ill formed, both alternatives take equal precedence
     *  OFvariant<OFString,OFBool>( "abc" ); // OK, but chooses OFBool!
     *  @endcode
     */
    template<typename T>
    OFvariant( T t );

    /** Destroys the value that the variant currently holds.
     */
    ~OFvariant();

    /** Copy assigns the value rhs holds to *this.
     *  @param rhs a const reference to another object of equal type.
     *  @pre all alternatives must be copy constructible and copy assignable.
     *  @return `*this`
     *  @post
     *  @li if `*this` and `rhs` hold the same alternative, the value contained in `rhs`
     *    is copy assigned to the value contained in `*this`.
     *  @li if `*this` and `rhs` hold different alternatives, the value contained in `*this`
     *    is destroyed and a new one is copy constructed from the value contained in `rhs`.
     */
    OFvariant& operator=( const OFvariant& rhs );

    /** Move assigns the value rhs holds to *this.
     *  @param rhs an rvalue reference to another object of equal type.
     *  @pre all alternatives must be move constructible and move assignable.
     *  @return `*this`
     *  @post
     *  @li if `*this` and `rhs` hold the same alternative, the value contained in `rhs`
     *    is move assigned to the value contained in `*this`.
     *  @li if `*this` and `rhs` hold different alternatives, the value contained in `*this`
     *    is destroyed and a new one is move constructed from the value contained in `rhs`.
     *  @note This constructor is currently only available if C++11 support was enabled.
     */
    OFvariant& operator=( OFvariant&& rhs );

    /** Converts the given argument to one of the alternatives and assigns it to *this.
     *  @tparam T the type of the argument, will be deduced automatically.
     *  @param t an object of type `T` that will be converted to one of the alternatives
     *    for assignment.
     *  @return `*this`
     *  @pre There must be at least one alternative that can be constructed from
     *    the given parameter `t` and there must be exactly one such alternative that
     *    takes precedence over the others.
     *  @attention `t` will be perfectly forwarded if C++11 support is available, i.e. the
     *    alternative may be move constructed from t if possible. Support for perfect
     *    forwarding is NOT available without C++11 support, therefore the alternative
     *    will be copy constructed in this case, this means: <b>the selected alternative
     *    must be copy constructible if pre C++11 compilers shall be supported</b>.
     *  @details
     *  <h3>Usage Example:</h3>
     *  @code{.cpp}
     *  OFvariant<int,float,long> v1;
     *  v1 = 3                         // OK
     *  OFvariant<int,int> v2;
     *  v2 = 3                         // ill formed, both alternatives take equal precedence
     *  OFvariant<OFString,OFBool> v3;
     *  v3 = "abc";                    // OK, but chooses OFBool!
     *  @endcode
     */
    template<typename T>
    OFvariant& operator=( T t );

    /** Get the index of alternative that is currently being held.
     *  @return the zero based index of that alternative that is currently being held by
     *    `*this`, i.e. `0` for the first alternative, `1` for the second, etc.
     */
    size_t index() const;
};

/** Try to get a pointer to the given alternative from an OFvariant object.
 *  @ingroup ofget_variant
 *  @relates OFvariant
 *  @tparam Alternative the chosen alternative that shall be accessed.
 *  @tparam Alternatives the alternatives the given variant could hold, will be deduced
 *    automatically.
 *  @param v a reference to an OFvariant object potentially holding `Alternative`.
 *  @return the address of the contained value of type `Alternative` if such a value is
 *    contained. `OFnullptr` otherwise.
 *  @details
 *  <h3>Usage Example:</h3>
 *  @code{.cpp}
 *  OFvariant<int,OFString> v;
 *  // ... some value is assigned to v ...
 *  if( int* pI = OFget<int>( v ) )
 *  {
 *    COUT << "Yes, it really is an int with the value " << *pI << '!' << OFendl;
 *    *pI = "27"; // now, let's directly assign something else
 *  }
 *  @endcode
 */
template<typename Alternative,typename... Alternatives>
Alternative* OFget( OFvariant<Alternatives...>* v );

/** Try to get a pointer to the given alternative from an OFvariant object.
 *  @ingroup ofget_variant
 *  @relates OFvariant
 *  @tparam Alternative the chosen alternative that shall be accessed.
 *  @tparam Alternatives the alternatives the given variant could hold, will be deduced
 *    automatically.
 *  @param v a const reference to an OFvariant object potentially holding `Alternative`.
 *  @return the address of the contained value of type `const Alternative` if such a value is
 *    contained. `OFnullptr` otherwise.
 *  @details
 *  <h3>Usage Example:</h3>
 *  @code{.cpp}
 *  const OFvariant<int,OFString> v( ... some value is assigned to v ... );
 *  if( int* pI = OFget<int>( v ) )        // error, the result is const!
 *  if( const int* pI = OFget<int>( v ) )  // OK
 *  {
 *    COUT << "Yes, it really is an int with the value " << *pI << '!' << OFendl;
 *    *pI = "27"; // Error, *pI is const!
 *  }
 *  @endcode
 */
template<typename Alternative,typename... Alternatives>
const Alternative* OFget( const OFvariant<Alternatives...>* v );

/** Applies the given visitor to the given OFvariant object.
 *  @ingroup ofvisit_variant
 *  @relates OFvariant
 *  @details@anchor ofget_alternative_const_variant
 *  @tparam Result the type of the returned value. Pre C++11 compiles do not allow determining the
 *    result type automatically in a portable way, therefore, it must be explicitly given by
 *    the caller.
 *  @tparam Visitor the type of the visitor, will be deduced automatically.
 *  @tparam Alternatives the alternatives the given variant could hold, will be deduced
 *    automatically.
 *  @param visitor the visitor that will be invoked with the alternative currently being
 *    held by the given OFvariant object.
 *  @param v a reference to an OFvariant object that is going to be visited.
 *  @return Let `CurrentAlternative` be the alternative that v currently holds: the result
 *    of `visitor( *OFget<CurrentAlternative>( &v ) )` will be converted to `Result` and then
 *    returned.
 *  @pre all possible results must be convertible to `Result`.
 *  @pre `visitor` must be able to take all possible alternatives.
 *  @note If C++11 support is available, the visitor will be forwarded using perfect forwarding.
 *    If not, the visitor may be copy constructed at least once, therefore, the visitor needs
 *    to be copy constructible when pre C++11 compilers are targeted.
 *  @details
 *  <h3>Usage Example:</h3>
 *  @code{.cpp}
 *  struct PowerVisitor
 *  {
 *      template<typename Number>
 *      Number operator()( Number number )
 *      {
 *          return number * number;
 *      }
 *  };
 *  struct PrintVisitor
 *  {
 *      template<typename Number>
 *      void operator()( Number number )
 *      {
 *          COUT << number << OFendl;
 *      }
 *  };
 *  struct AssignVisitor
 *  {
 *      template<typename Number>
 *      void operator()( Number& number )
 *      {
 *          number *= number;
 *      }
 *  };
 *  // ...
 *  OFvariant<int,float,double> v( 3.14 );
 *  OFvariant<int,float,double> result = OFvisit<OFvariant<int,float,double> >( PowerVisitor(), v );
 *  switch( result.index() )
 *  {
 *  case 0: COUT << "int "; break;
 *  case 1: COUT << "float "; break;
 *  case 2: COUT << "double "; break;
 *  }
 *  OFvisit<void>( PrintVisitor(), result );
 *  COUT << "double " << OFvisit<double>( PowerVisitor(), v ) << OFendl;   // OK, every alternative fits inside double
 *  COUT << "int " << OFvisit<int>( PowerVisitor(), v ) << OFendl;         // OK, value will be truncated!
 *  COUT << "string " << OFvisit<OFString>( PowerVisitor(), v ) << OFendl; // ERROR!
 *  OFvisit<void>( AssignVisitor(), v );
 *  OFvisit<void>( PrintVisitor(), v );
 *  @endcode
 *  <b>Output (if the error was removed):</b>
 *  @verbatim
    double 9.8596
    double 9.8596
    int 9
    9.8596
    @endverbatim
 */
template<typename Result,typename Visitor,typename... Alternatives>
Result OFvisit( Visitor visitor, OFvariant<Alternatives...>& v );

/** Applies the given visitor to the given OFvariant object.
 *  @ingroup ofvisit_variant
 *  @relates OFvariant
 *  @tparam Result the type of the returned value. Pre C++11 compiles do not allow determining the
 *    result type automatically in a portable way, therefore, it must be explicitly given by
 *    the caller.
 *  @tparam Visitor the type of the visitor, will be deduced automatically.
 *  @tparam Alternatives the alternatives the given variant could hold, will be deduced
 *    automatically.
 *  @param visitor the visitor that will be invoked with the alternative currently being
 *    held by the given OFvariant object.
 *  @param v a const reference to an OFvariant object that is going to be visited.
 *  @return Let `CurrentAlternative` be the alternative that v currently holds: the result
 *    of `visitor( *OFget<CurrentAlternative>( &v ) )` will be converted to `Result` and then
 *    returned.
 *  @pre all possible results must be convertible to `Result`.
 *  @pre `visitor` must be able to take all possible alternatives.
 *  @note If C++11 support is available, the visitor will be forwarded using perfect forwarding.
 *    If not, the visitor may be copy constructed at least once, therefore, the visitor needs
 *    to be copy constructible when pre C++11 compilers are targeted.
 *  @details
 *  <h3>Usage Example:</h3>
 *  @code{.cpp}
 *  struct PowerVisitor
 *  {
 *      template<typename Number>
 *      Number operator()( Number number )
 *      {
 *          return number * number;
 *      }
 *  };
 *  struct PrintVisitor
 *  {
 *      template<typename Number>
 *      void operator()( Number number )
 *      {
 *          COUT << number << OFendl;
 *      }
 *  };
 *  struct AssignVisitor
 *  {
 *      template<typename Number>
 *      void operator()( Number& number )
 *      {
 *          number *= number;
 *      }
 *  };
 *  // ...
 *  const OFvariant<int,float,double> v( 3.14 );
 *  OFvariant<int,float,double> result = OFvisit<OFvariant<int,float,double> >( PowerVisitor(), v );
 *  switch( result.index() )
 *  {
 *  case 0: COUT << "int "; break;
 *  case 1: COUT << "float "; break;
 *  case 2: COUT << "double "; break;
 *  }
 *  OFvisit<void>( PrintVisitor(), result );
 *  COUT << "double " << OFvisit<double>( PowerVisitor(), v ) << OFendl;   // OK, every alternative fits inside double
 *  COUT << "int " << OFvisit<int>( PowerVisitor(), v ) << OFendl;         // OK, value will be truncated!
 *  COUT << "string " << OFvisit<OFString>( PowerVisitor(), v ) << OFendl; // ERROR!
 *  OFvisit<void>( AssignVisitor(), v );                                   // ERROR, v is const!
 *  OFvisit<void>( PrintVisitor(), v );
 *  @endcode
 *  <b>Output (if the errors were removed):</b>
 *  @verbatim
    double 9.8596
    double 9.8596
    int 9
    3.14
    @endverbatim
 */
template<typename Result,typename Visitor,typename... Alternatives>
Result OFvisit( Visitor visitor, const OFvariant<Alternatives...>& v );

#endif // DOXYGEN

/** A helper type for making OFvariant default constructible.
 *  @relates OFvariant
 *  @details
 *  Use OFmonostate as the first alternative of an OFvariant otherwise holding a non default constructible type as
 *  the first alternative for making the variant itself default constructible.
 *  @note Be aware that any visitor applied to such an OFvariant object must also accept OFmonostate as an argument.
 *  @details
 *  <h3>Example</h3>
 *  @code{.cpp}
 *  template<typename T>
 *  struct NonDefaultConstructible
 *  {
 *      NonDefaultConstructible( T t ) : value( t ) {}
 *      T value;
 *  };
 *  typedef NonDefaultConstructible<int>   nint;
 *  typedef NonDefaultConstructible<float> nfloat;
 *  // ...
 *  OFvariant<nint,nfloat> v( 3 );        // OK, but what if we don't know the value yet?
 *  OFvariant<nint,nfloat> v;             // ERROR!
 *  OFvariant<OFmonostate,nint,nfloat> v; // OK
 *  v = 3;                                // assign the value when it's known
 *  @endcode
 */
struct OFmonostate {};

#endif // OFVARIANT_H
