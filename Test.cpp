#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "doctest.h"
#include "OrgChart.hpp"

using namespace std;
using namespace ariel;

TEST_CASE("Construct a new Chart"){

    /*
                   A
                 / | \
                /  |   \
              B    C     D
             / \        /  \
            E    F     O    P
            |        / | \
            G       R  S  T
            |
            H
    */

    OrgChart alphabet;
    CHECK_NOTHROW(alphabet.add_root("A"));
    CHECK_NOTHROW(alphabet.add_sub("A", "B"));
    CHECK_NOTHROW(alphabet.add_sub("A", "C"));
    CHECK_NOTHROW(alphabet.add_sub("A", "D"));
    CHECK_NOTHROW(alphabet.add_sub("D", "O"));
    CHECK_NOTHROW(alphabet.add_sub("O", "R"));
    CHECK_NOTHROW(alphabet.add_sub("O", "S"));
    CHECK_NOTHROW(alphabet.add_sub("O", "T"));
    CHECK_NOTHROW(alphabet.add_sub("D", "P"));
    CHECK_NOTHROW(alphabet.add_sub("B", "E"));
    CHECK_NOTHROW(alphabet.add_sub("B", "F"));
    CHECK_NOTHROW(alphabet.add_sub("E", "G"));
    CHECK_NOTHROW(alphabet.add_sub("G", "H"));


    cout << alphabet << endl;

    /*
             +- A
                +- B
                |  +- E
                |  |  +- G
                |  |     +- H
                |  +- F
                +- C
                +- D
                    +- O
                    |  +- R
                    |  +- S
                    |  +- T
                    +- P


    */
  

    for (auto it = alphabet.begin_level_order(); it != alphabet.end_level_order(); ++it)
   {
    cout << (*it) << endl;
   }  
 
    vector<string> vector1 = {"A","B","E","G","H","F","C","D","O","R","S","T","P"};
    auto iter1 = alphabet.begin_level_order();
    unsigned int i = 0;
    while (iter1 != alphabet.end_level_order())
    {
        CHECK((*iter1) == vector1[i]);
        i++;
        ++iter1;
    }

    

}

TEST_CASE("simple iterators") {

    SUBCASE("==") {

        OrgChart School;
        School.add_root("Director").add_sub("Director", "Secretary");
        auto it1 = School.begin();
        auto it2 = School.begin_level_order();
        auto it3 = School.begin_reverse_order();
        auto it4 = School.begin_preorder();
        CHECK(it1 == it3);
        CHECK(it1 == it2);
        CHECK(it2 == it4);

    }

    SUBCASE("++") {

        OrgChart School;
        School.add_root("Director").add_sub("Director", "Secretary").add_sub("Director", "Supervisor");
        auto it1 = School.begin();
        auto it2 = School.begin_level_order();
        auto it3 = School.begin_reverse_order();
        auto it4 = School.begin_preorder();

       
    }
}

TEST_CASE("Check all traversals order")
{

    /*
        "one"
            \
            "two"
                \
                "three"
                    \
                    "four"
                        \
                        "five"
                            \
                            "six"
                                \
                                "seven"
                                    \
                                    "eight"
                                        \
                                        "nine"
                                            \
                                            "ten"

    */

    OrgChart numbers;
    CHECK_NOTHROW(numbers.add_root("one"));
    CHECK_NOTHROW(numbers.add_sub("one", "two"));
    CHECK_NOTHROW(numbers.add_sub("two", "three"));
    CHECK_NOTHROW(numbers.add_sub("three", "four"));
    CHECK_NOTHROW(numbers.add_sub("four", "five"));
    CHECK_NOTHROW(numbers.add_sub("five", "six"));
    CHECK_NOTHROW(numbers.add_sub("six", "seven"));
    CHECK_NOTHROW(numbers.add_sub("seven", "eight"));
    CHECK_NOTHROW(numbers.add_sub("eight", "nine"));
    CHECK_NOTHROW(numbers.add_sub("nine", "ten"));

   


    vector<string> temp_vector1 = {"ten", "nine", "eight", "seven", "six", "five", "four", "three", "two", "one"};
    auto it_rev = numbers.begin_reverse_order();
    unsigned int i = 0;
    while (it_rev != numbers.end_reverse_order())
    {
        CHECK((*it_rev) == temp_vector1[i]);
        i++;
        ++it_rev;
    }

    vector<string> temp_vector2 = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten"};
    auto it_lev = numbers.begin_level_order();
    i = 0;
    while (it_lev != numbers.end_level_order())
    {
        CHECK((*it_lev) == temp_vector2[i]);
        i++;
        it_lev++;
    }

    i = 0;

    for (string element : numbers)
    {
        CHECK(element == temp_vector2[i]);
        i++;
    }

    auto itPost = numbers.begin_preorder();
    i = 0;
    while (itPost != numbers.end_preorder())
    {
        CHECK((*itPost) == temp_vector2[i]);
        i++;
        itPost++;
    }

   cout << numbers << endl;

    /*
        +- one
            +- two
                +- three
                    +- four
                        +- five
                            +- six
                                +- seven
                                    +- eight
                                        +- nine
                                            +- ten

    */
}
