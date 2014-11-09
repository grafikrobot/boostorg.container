////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2004-2013. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/container for documentation.
//
////////////////////////////////////////

#ifndef BOOST_CONTAINER_TEST_SET_TEST_HEADER
#define BOOST_CONTAINER_TEST_SET_TEST_HEADER

#include <boost/container/detail/config_begin.hpp>
#include "check_equal_containers.hpp"
#include <memory>
#include <set>
#include <functional>
#include "print_container.hpp"
#include <boost/move/utility_core.hpp>
#include <boost/move/iterator.hpp>
#include <boost/move/make_unique.hpp>
#include <string>

#include <boost/intrusive/detail/has_member_function_callable_with.hpp>
#define BOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_FUNCNAME rebalance
#define BOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_NS_BEGIN namespace boost { namespace container { namespace test {
#define BOOST_INTRUSIVE_HAS_MEMBER_FUNCTION_CALLABLE_WITH_NS_END   }}}
#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, 0, <boost/intrusive/detail/has_member_function_callable_with.hpp>))
#include BOOST_PP_ITERATE()


namespace boost{
namespace container {
namespace test{

template<class C>
void set_test_rebalanceable(C &, boost::container::container_detail::false_type)
{}

template<class C>
void set_test_rebalanceable(C &c, boost::container::container_detail::true_type)
{
   c.rebalance();
}

template<class MyBoostSet
        ,class MyStdSet
        ,class MyBoostMultiSet
        ,class MyStdMultiSet>
int set_test_copyable(boost::container::container_detail::false_type)
{  return 0; }

template<class MyBoostSet
        ,class MyStdSet
        ,class MyBoostMultiSet
        ,class MyStdMultiSet>
int set_test_copyable(boost::container::container_detail::true_type)
{
   typedef typename MyBoostSet::value_type IntType;
   const int max = 50;

   ::boost::movelib::unique_ptr<MyBoostSet> const pboostset = ::boost::movelib::make_unique<MyBoostSet>();
   ::boost::movelib::unique_ptr<MyStdSet>   const pstdset = ::boost::movelib::make_unique<MyStdSet>();
   ::boost::movelib::unique_ptr<MyBoostMultiSet> const pboostmultiset = ::boost::movelib::make_unique<MyBoostMultiSet>();
   ::boost::movelib::unique_ptr<MyStdMultiSet>   const pstdmultiset   = ::boost::movelib::make_unique<MyStdMultiSet>();

   MyBoostSet &boostset = *pboostset;
   MyStdSet   &stdset   = *pstdset;
   MyBoostMultiSet &boostmultiset = *pboostmultiset;
   MyStdMultiSet   &stdmultiset   = *pstdmultiset;

   for(int i = 0; i < max; ++i){
      IntType move_me(i);
      boostset.insert(boost::move(move_me));
      stdset.insert(i);
      IntType move_me2(i);
      boostmultiset.insert(boost::move(move_me2));
      stdmultiset.insert(i);
   }
   if(!CheckEqualContainers(boostset, stdset)) return 1;
   if(!CheckEqualContainers(boostmultiset, stdmultiset)) return 1;

   {
      //Now, test copy constructor
      MyBoostSet boostsetcopy(boostset);
      MyStdSet stdsetcopy(stdset);

      if(!CheckEqualContainers(boostsetcopy, stdsetcopy))
         return 1;

      MyBoostMultiSet boostmsetcopy(boostmultiset);
      MyStdMultiSet stdmsetcopy(stdmultiset);

      if(!CheckEqualContainers(boostmsetcopy, stdmsetcopy))
         return 1;

      //And now assignment
      boostsetcopy  =boostset;
      stdsetcopy  = stdset;

      if(!CheckEqualContainers(boostsetcopy, stdsetcopy))
         return 1;

      boostmsetcopy = boostmultiset;
      stdmsetcopy = stdmultiset;

      if(!CheckEqualContainers(boostmsetcopy, stdmsetcopy))
         return 1;
   }
   return 0;
}


template<class MyBoostSet
        ,class MyStdSet
        ,class MyBoostMultiSet
        ,class MyStdMultiSet>
int set_test ()
{
   typedef typename MyBoostSet::value_type IntType;
   const int max = 50;

   ::boost::movelib::unique_ptr<MyBoostSet> const pboostset = ::boost::movelib::make_unique<MyBoostSet>();
   ::boost::movelib::unique_ptr<MyStdSet>   const pstdset = ::boost::movelib::make_unique<MyStdSet>();
   ::boost::movelib::unique_ptr<MyBoostMultiSet> const pboostmultiset = ::boost::movelib::make_unique<MyBoostMultiSet>();
   ::boost::movelib::unique_ptr<MyStdMultiSet>   const pstdmultiset   = ::boost::movelib::make_unique<MyStdMultiSet>();

   MyBoostSet &boostset = *pboostset;
   MyStdSet   &stdset   = *pstdset;
   MyBoostMultiSet &boostmultiset = *pboostmultiset;
   MyStdMultiSet   &stdmultiset   = *pstdmultiset;

   //Test construction from a range
   {
      IntType aux_vect[50];
      for(int i = 0; i < 50; ++i){
         IntType move_me(i/2);
         aux_vect[i] = boost::move(move_me);
      }
      int aux_vect2[50];
      for(int i = 0; i < 50; ++i){
         aux_vect2[i] = i/2;
      }
      IntType aux_vect3[50];
      for(int i = 0; i < 50; ++i){
         IntType move_me(i/2);
         aux_vect3[i] = boost::move(move_me);
      }

      ::boost::movelib::unique_ptr<MyBoostSet> const pboostset2 = ::boost::movelib::make_unique<MyBoostSet>
            ( boost::make_move_iterator(&aux_vect[0])
            , boost::make_move_iterator(aux_vect + 50));
      ::boost::movelib::unique_ptr<MyStdSet>  const pstdset2 = ::boost::movelib::make_unique<MyStdSet>
            (&aux_vect2[0], &aux_vect2[0] + 50);
      ::boost::movelib::unique_ptr<MyBoostMultiSet> const pboostmultiset2 = ::boost::movelib::make_unique<MyBoostMultiSet>
            ( boost::make_move_iterator(&aux_vect3[0])
            , boost::make_move_iterator(aux_vect3 + 50));
      ::boost::movelib::unique_ptr<MyStdMultiSet>   const pstdmultiset2   = ::boost::movelib::make_unique<MyStdMultiSet>
            (&aux_vect2[0], &aux_vect2[0] + 50);

      MyBoostSet &boostset2 = *pboostset2;
      MyStdSet   &stdset2   = *pstdset2;
      MyBoostMultiSet &boostmultiset2 = *pboostmultiset2;
      MyStdMultiSet   &stdmultiset2   = *pstdmultiset2;

      if(!CheckEqualContainers(boostset2, stdset2)){
         std::cout << "Error in construct<MyBoostSet>(MyBoostSet2)" << std::endl;
         return 1;
      }
      if(!CheckEqualContainers(boostmultiset2, stdmultiset2)){
         std::cout << "Error in construct<MyBoostMultiSet>(MyBoostMultiSet2)" << std::endl;
         return 1;
      }

      //ordered range insertion
      for(int i = 0; i < 50; ++i){
         IntType move_me(i);
         aux_vect[i] = boost::move(move_me);
      }

      for(int i = 0; i < 50; ++i){
         aux_vect2[i] = i;
      }

      for(int i = 0; i < 50; ++i){
         IntType move_me(i);
         aux_vect3[i] = boost::move(move_me);
      }

      //some comparison operators
      if(!(boostset2 == boostset2))
         return 1;
      if(boostset2 != boostset2)
         return 1;
      if(boostset2 < boostset2)
         return 1;
      if(boostset2 > boostset2)
         return 1;
      if(!(boostset2 <= boostset2))
         return 1;
      if(!(boostset2 >= boostset2))
         return 1;

      ::boost::movelib::unique_ptr<MyBoostSet> const pboostset3 = ::boost::movelib::make_unique<MyBoostSet>
            ( ordered_unique_range
            , boost::make_move_iterator(&aux_vect[0])
            , boost::make_move_iterator(&aux_vect[0] + 50));
      ::boost::movelib::unique_ptr<MyStdSet>   const pstdset3 = ::boost::movelib::make_unique<MyStdSet>
            (&aux_vect2[0], &aux_vect2[0] + 50);
      ::boost::movelib::unique_ptr<MyBoostMultiSet> const pboostmultiset3 = ::boost::movelib::make_unique<MyBoostMultiSet>
            ( ordered_range
            , boost::make_move_iterator(&aux_vect3[0])
            , boost::make_move_iterator(aux_vect3 + 50));
      ::boost::movelib::unique_ptr<MyStdMultiSet>   const pstdmultiset3   = ::boost::movelib::make_unique<MyStdMultiSet>
            (&aux_vect2[0], &aux_vect2[0] + 50);

      MyBoostSet &boostset3 = *pboostset3;
      MyStdSet   &stdset3   = *pstdset3;
      MyBoostMultiSet &boostmultiset3 = *pboostmultiset3;
      MyStdMultiSet   &stdmultiset3   = *pstdmultiset3;

      if(!CheckEqualContainers(boostset3, stdset3)){
         std::cout << "Error in construct<MyBoostSet>(MyBoostSet3)" << std::endl;
         return 1;
      }
      if(!CheckEqualContainers(boostmultiset3, stdmultiset3)){
         std::cout << "Error in construct<MyBoostMultiSet>(MyBoostMultiSet3)" << std::endl;
         return 1;
      }
   }

   for(int i = 0; i < max; ++i){
      IntType move_me(i);
      boostset.insert(boost::move(move_me));
      stdset.insert(i);
      boostset.insert(IntType(i));
      stdset.insert(i);
      IntType move_me2(i);
      boostmultiset.insert(boost::move(move_me2));
      stdmultiset.insert(i);
      boostmultiset.insert(IntType(i));
      stdmultiset.insert(i);
   }

   if(!CheckEqualContainers(boostset, stdset)){
      std::cout << "Error in boostset.insert(boost::move(move_me)" << std::endl;
      return 1;
   }

   if(!CheckEqualContainers(boostmultiset, stdmultiset)){
      std::cout << "Error in boostmultiset.insert(boost::move(move_me)" << std::endl;
      return 1;
   }

   typename MyBoostSet::iterator it = boostset.begin();
   typename MyBoostSet::const_iterator cit = it;
   (void)cit;

   boostset.erase(boostset.begin());
   stdset.erase(stdset.begin());
   boostmultiset.erase(boostmultiset.begin());
   stdmultiset.erase(stdmultiset.begin());
   if(!CheckEqualContainers(boostset, stdset)){
      std::cout << "Error in boostset.erase(boostset.begin())" << std::endl;
      return 1;
   }
   if(!CheckEqualContainers(boostmultiset, stdmultiset)){
      std::cout << "Error in boostmultiset.erase(boostmultiset.begin())" << std::endl;
      return 1;
   }

   boostset.erase(boostset.begin());
   stdset.erase(stdset.begin());
   boostmultiset.erase(boostmultiset.begin());
   stdmultiset.erase(stdmultiset.begin());
   if(!CheckEqualContainers(boostset, stdset)){
      std::cout << "Error in boostset.erase(boostset.begin())" << std::endl;
      return 1;
   }
   if(!CheckEqualContainers(boostmultiset, stdmultiset)){
      std::cout << "Error in boostmultiset.erase(boostmultiset.begin())" << std::endl;
      return 1;
   }

   //Swapping test
   MyBoostSet tmpboosteset2;
   MyStdSet tmpstdset2;
   MyBoostMultiSet tmpboostemultiset2;
   MyStdMultiSet tmpstdmultiset2;
   boostset.swap(tmpboosteset2);
   stdset.swap(tmpstdset2);
   boostmultiset.swap(tmpboostemultiset2);
   stdmultiset.swap(tmpstdmultiset2);
   boostset.swap(tmpboosteset2);
   stdset.swap(tmpstdset2);
   boostmultiset.swap(tmpboostemultiset2);
   stdmultiset.swap(tmpstdmultiset2);
   if(!CheckEqualContainers(boostset, stdset)){
      std::cout << "Error in boostset.swap(tmpboosteset2)" << std::endl;
      return 1;
   }
   if(!CheckEqualContainers(boostmultiset, stdmultiset)){
      std::cout << "Error in boostmultiset.swap(tmpboostemultiset2)" << std::endl;
      return 1;
   }

   //Insertion from other container
   //Initialize values
   {
      IntType aux_vect[50];
      for(int i = 0; i < 50; ++i){
         IntType move_me(-1);
         aux_vect[i] = boost::move(move_me);
      }
      int aux_vect2[50];
      for(int i = 0; i < 50; ++i){
         aux_vect2[i] = -1;
      }
      IntType aux_vect3[50];
      for(int i = 0; i < 50; ++i){
         IntType move_me(-1);
         aux_vect3[i] = boost::move(move_me);
      }

      boostset.insert(boost::make_move_iterator(&aux_vect[0]), boost::make_move_iterator(&aux_vect[0] + 50));
      stdset.insert(&aux_vect2[0], &aux_vect2[0] + 50);
      boostmultiset.insert(boost::make_move_iterator(&aux_vect3[0]), boost::make_move_iterator(aux_vect3 + 50));
      stdmultiset.insert(&aux_vect2[0], &aux_vect2[0] + 50);
      if(!CheckEqualContainers(boostset, stdset)){
         std::cout << "Error in boostset.insert(boost::make_move_iterator(&aux_vect[0])..." << std::endl;
         return 1;
      }
      if(!CheckEqualContainers(boostmultiset, stdmultiset)){
         std::cout << "Error in boostmultiset.insert(boost::make_move_iterator(&aux_vect3[0]), ..." << std::endl;
         return 1;
      }

      for(int i = 0, j = static_cast<int>(boostset.size()); i < j; ++i){
         IntType erase_me(i);
         boostset.erase(erase_me);
         stdset.erase(i);
         boostmultiset.erase(erase_me);
         stdmultiset.erase(i);
         if(!CheckEqualContainers(boostset, stdset)){
            std::cout << "Error in boostset.erase(erase_me)" << boostset.size() << " " << stdset.size() << std::endl;
            return 1;
         }
         if(!CheckEqualContainers(boostmultiset, stdmultiset)){
            std::cout << "Error in boostmultiset.erase(erase_me)" << std::endl;
            return 1;
         }
      }
   }
   {
      IntType aux_vect[50];
      for(int i = 0; i < 50; ++i){
         IntType move_me(-1);
         aux_vect[i] = boost::move(move_me);
      }
      int aux_vect2[50];
      for(int i = 0; i < 50; ++i){
         aux_vect2[i] = -1;
      }
      IntType aux_vect3[50];
      for(int i = 0; i < 50; ++i){
         IntType move_me(-1);
         aux_vect3[i] = boost::move(move_me);
      }

      IntType aux_vect4[50];
      for(int i = 0; i < 50; ++i){
         IntType move_me(-1);
         aux_vect4[i] = boost::move(move_me);
      }

      IntType aux_vect5[50];
      for(int i = 0; i < 50; ++i){
         IntType move_me(-1);
         aux_vect5[i] = boost::move(move_me);
      }

      boostset.insert(boost::make_move_iterator(&aux_vect[0]), boost::make_move_iterator(&aux_vect[0] + 50));
      boostset.insert(boost::make_move_iterator(&aux_vect3[0]), boost::make_move_iterator(&aux_vect3[0] + 50));
      stdset.insert(&aux_vect2[0], &aux_vect2[0] + 50);
      stdset.insert(&aux_vect2[0], &aux_vect2[0] + 50);
      boostmultiset.insert(boost::make_move_iterator(&aux_vect4[0]), boost::make_move_iterator(&aux_vect4[0] + 50));
      boostmultiset.insert(boost::make_move_iterator(&aux_vect5[0]), boost::make_move_iterator(&aux_vect5[0] + 50));
      stdmultiset.insert(&aux_vect2[0], &aux_vect2[0] + 50);
      stdmultiset.insert(&aux_vect2[0], &aux_vect2[0] + 50);
      if(!CheckEqualContainers(boostset, stdset)){
         std::cout << "Error in boostset.insert(boost::make_move_iterator(&aux_vect3[0])..." << std::endl;
         return 1;
      }
      if(!CheckEqualContainers(boostmultiset, stdmultiset)){
         std::cout << "Error in boostmultiset.insert(boost::make_move_iterator(&aux_vect5[0])..." << std::endl;
         return 1;
      }

      boostset.erase(*boostset.begin());
      stdset.erase(*stdset.begin());
      boostmultiset.erase(*boostmultiset.begin());
      stdmultiset.erase(*stdmultiset.begin());
      if(!CheckEqualContainers(boostset, stdset)){
         std::cout << "Error in boostset.erase(*boostset.begin())" << std::endl;
         return 1;
      }
      if(!CheckEqualContainers(boostmultiset, stdmultiset)){
         std::cout << "Error in boostmultiset.erase(*boostmultiset.begin())" << std::endl;
         return 1;
      }
   }

   for(int i = 0; i < max; ++i){
      IntType move_me(i);
      boostset.insert(boost::move(move_me));
      stdset.insert(i);
      IntType move_me2(i);
      boostmultiset.insert(boost::move(move_me2));
      stdmultiset.insert(i);
   }

   if(!CheckEqualContainers(boostset, stdset)){
      std::cout << "Error in boostset.insert(boost::move(move_me)) try 2" << std::endl;
      return 1;
   }
   if(!CheckEqualContainers(boostmultiset, stdmultiset)){
      std::cout << "Error in boostmultiset.insert(boost::move(move_me2)) try 2" << std::endl;
      return 1;
   }

   for(int i = 0; i < max; ++i){
      {
         IntType move_me(i);
         boostset.insert(boostset.begin(), boost::move(move_me));
         stdset.insert(stdset.begin(), i);
         //PrintContainers(boostset, stdset);
         IntType move_me2(i);
         boostmultiset.insert(boostmultiset.begin(), boost::move(move_me2));
         stdmultiset.insert(stdmultiset.begin(), i);
         //PrintContainers(boostmultiset, stdmultiset);
         if(!CheckEqualContainers(boostset, stdset)){
            std::cout << "Error in boostset.insert(boostset.begin(), boost::move(move_me))" << std::endl;
            return 1;
         }
         if(!CheckEqualContainers(boostmultiset, stdmultiset)){
            std::cout << "Error in boostmultiset.insert(boostmultiset.begin(), boost::move(move_me2))" << std::endl;
            return 1;
         }

         IntType move_me3(i);
         boostset.insert(boostset.end(), boost::move(move_me3));
         stdset.insert(stdset.end(), i);
         IntType move_me4(i);
         boostmultiset.insert(boostmultiset.end(), boost::move(move_me4));
         stdmultiset.insert(stdmultiset.end(), i);
         if(!CheckEqualContainers(boostset, stdset)){
            std::cout << "Error in boostset.insert(boostset.end(), boost::move(move_me3))" << std::endl;
            return 1;
         }
         if(!CheckEqualContainers(boostmultiset, stdmultiset)){
            std::cout << "Error in boostmultiset.insert(boostmultiset.end(), boost::move(move_me4))" << std::endl;
            return 1;
         }
      }
      {
         IntType move_me(i);
         boostset.insert(boostset.upper_bound(move_me), boost::move(move_me));
         stdset.insert(stdset.upper_bound(i), i);
         //PrintContainers(boostset, stdset);
         IntType move_me2(i);
         boostmultiset.insert(boostmultiset.upper_bound(move_me2), boost::move(move_me2));
         stdmultiset.insert(stdmultiset.upper_bound(i), i);
         //PrintContainers(boostmultiset, stdmultiset);
         if(!CheckEqualContainers(boostset, stdset)){
            std::cout << "Error in boostset.insert(boostset.upper_bound(move_me), boost::move(move_me))" << std::endl;
            return 1;
         }
         if(!CheckEqualContainers(boostmultiset, stdmultiset)){
            std::cout << "Error in boostmultiset.insert(boostmultiset.upper_bound(move_me2), boost::move(move_me2))" << std::endl;
            return 1;
         }

      }
      {
         IntType move_me(i);
         IntType move_me2(i);
         boostset.insert(boostset.lower_bound(move_me), boost::move(move_me2));
         stdset.insert(stdset.lower_bound(i), i);
         //PrintContainers(boostset, stdset);
         move_me2 = i;
         boostmultiset.insert(boostmultiset.lower_bound(move_me2), boost::move(move_me2));
         stdmultiset.insert(stdmultiset.lower_bound(i), i);
         //PrintContainers(boostmultiset, stdmultiset);
         if(!CheckEqualContainers(boostset, stdset)){
            std::cout << "Error in boostset.insert(boostset.lower_bound(move_me), boost::move(move_me2))" << std::endl;
            return 1;
         }
         if(!CheckEqualContainers(boostmultiset, stdmultiset)){
            std::cout << "Error in boostmultiset.insert(boostmultiset.lower_bound(move_me2), boost::move(move_me2))" << std::endl;
            return 1;
         }
         set_test_rebalanceable(boostset
            , container_detail::bool_<has_member_function_callable_with_rebalance<MyBoostSet>::value>());
         if(!CheckEqualContainers(boostset, stdset)){
            std::cout << "Error in boostset.rebalance()" << std::endl;
            return 1;
         }
         set_test_rebalanceable(boostmultiset
            , container_detail::bool_<has_member_function_callable_with_rebalance<MyBoostMultiSet>::value>());
         if(!CheckEqualContainers(boostmultiset, stdmultiset)){
            std::cout << "Error in boostmultiset.rebalance()" << std::endl;
            return 1;
         }
      }
   }

   //Compare count with std containers
   for(int i = 0; i < max; ++i){
      IntType count_me(i);
      if(boostset.count(count_me) != stdset.count(i)){
         return -1;
      }
      if(boostmultiset.count(count_me) != stdmultiset.count(i)){
         return -1;
      }
   }

   //Compare find/lower_bound/upper_bound in set
   {
      typename MyBoostSet::iterator bs_b = boostset.begin();
      typename MyBoostSet::iterator bs_e = boostset.end();
      typename MyStdSet::iterator ss_b   = stdset.begin();

      std::size_t i = 0;
      while(bs_b != bs_e){
         ++i;
         typename MyBoostSet::iterator bs_i;
         typename MyStdSet::iterator ss_i;
         //find
         bs_i = boostset.find(*bs_b);
         ss_i = stdset.find(*ss_b);
         if(!CheckEqualIt(bs_i, ss_i, boostset, stdset)){
            return -1;
         }
         //lower bound
         bs_i = boostset.lower_bound(*bs_b);
         ss_i = stdset.lower_bound(*ss_b);
         if(!CheckEqualIt(bs_i, ss_i, boostset, stdset)){
            return -1;
         }
         //upper bound
         bs_i = boostset.upper_bound(*bs_b);
         ss_i = stdset.upper_bound(*ss_b);
         if(!CheckEqualIt(bs_i, ss_i, boostset, stdset)){
            return -1;
         }
         //equal range
         std::pair<typename MyBoostSet::iterator
                  ,typename MyBoostSet::iterator> bs_ip;
         std::pair<typename MyStdSet::iterator
                  ,typename MyStdSet::iterator>   ss_ip;
         bs_ip = boostset.equal_range(*bs_b);
         ss_ip = stdset.equal_range(*ss_b);
         if(!CheckEqualIt(bs_ip.first, ss_ip.first, boostset, stdset)){
            return -1;
         }
         if(!CheckEqualIt(bs_ip.second, ss_ip.second, boostset, stdset)){
            return -1;
         }
         ++bs_b;
         ++ss_b;
      }
   }
   //Compare find/lower_bound/upper_bound in multiset
   {
      typename MyBoostMultiSet::iterator bm_b = boostmultiset.begin();
      typename MyBoostMultiSet::iterator bm_e = boostmultiset.end();
      typename MyStdMultiSet::iterator sm_b   = stdmultiset.begin();

      while(bm_b != bm_e){
         typename MyBoostMultiSet::iterator bm_i;
         typename MyStdMultiSet::iterator sm_i;
         //find
         bm_i = boostmultiset.find(*bm_b);
         sm_i = stdmultiset.find(*sm_b);
         if(!CheckEqualIt(bm_i, sm_i, boostmultiset, stdmultiset)){
            return -1;
         }
         //lower bound
         bm_i = boostmultiset.lower_bound(*bm_b);
         sm_i = stdmultiset.lower_bound(*sm_b);
         if(!CheckEqualIt(bm_i, sm_i, boostmultiset, stdmultiset)){
            return -1;
         }
         //upper bound
         bm_i = boostmultiset.upper_bound(*bm_b);
         sm_i = stdmultiset.upper_bound(*sm_b);
         if(!CheckEqualIt(bm_i, sm_i, boostmultiset, stdmultiset)){
            return -1;
         }
         //equal range
         std::pair<typename MyBoostMultiSet::iterator
                  ,typename MyBoostMultiSet::iterator> bm_ip;
         std::pair<typename MyStdMultiSet::iterator
                  ,typename MyStdMultiSet::iterator>   sm_ip;
         bm_ip = boostmultiset.equal_range(*bm_b);
         sm_ip = stdmultiset.equal_range(*sm_b);
         if(!CheckEqualIt(bm_ip.first, sm_ip.first, boostmultiset, stdmultiset)){
            return -1;
         }
         if(!CheckEqualIt(bm_ip.second, sm_ip.second, boostmultiset, stdmultiset)){
            return -1;
         }
         ++bm_b;
         ++sm_b;
      }
   }

   //Now do count exercise
   boostset.erase(boostset.begin(), boostset.end());
   boostmultiset.erase(boostmultiset.begin(), boostmultiset.end());
   boostset.clear();
   boostmultiset.clear();

   for(int j = 0; j < 3; ++j)
   for(int i = 0; i < 100; ++i){
      IntType move_me(i);
      boostset.insert(boost::move(move_me));
      IntType move_me2(i);
      boostmultiset.insert(boost::move(move_me2));
      IntType count_me(i);
      if(boostset.count(count_me) != typename MyBoostMultiSet::size_type(1)){
         std::cout << "Error in boostset.count(count_me)" << std::endl;
         return 1;
      }
      if(boostmultiset.count(count_me) != typename MyBoostMultiSet::size_type(j+1)){
         std::cout << "Error in boostmultiset.count(count_me)" << std::endl;
         return 1;
      }
   }

   if(set_test_copyable<MyBoostSet, MyStdSet, MyBoostMultiSet, MyStdMultiSet>
      (container_detail::bool_<boost::container::test::is_copyable<IntType>::value>())){
      return 1;
   }

   return 0;
}

}  //namespace test{
}  //namespace container {
}  //namespace boost{

#include <boost/container/detail/config_end.hpp>

#endif
