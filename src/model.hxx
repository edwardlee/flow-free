#pragma once

#include <ge211.hxx>

#include <iostream>
#include <vector>
#include <unordered_map>

class Model
{
public:
    /// Model dimensions will use `int` coordinates.
    using Dimensions = ge211::Dims<int>;

    /// Model positions will use `int` coordinates.
    using Position = ge211::Posn<int>;

    /// Model twovecs are two dimensional vectors of ints.
    using TwoVec = std::vector<std::vector<int>>;

    /// Constructs a model with `size` as both its width and height.
    ///
    /// ERRORS:
    ///  - Throws `ge211::Client_logic_error` if `size` is less
    ///    than 4 or greater than 8.
    explicit Model(int size = 8);

    /// Constructs a model with the given width and height.
    ///
    /// ERRORS:
    ///  - Throws `ge211::Client_logic_error` if either dimension is less
    ///    than 4 or greater than 8.
    Model(int width, int height);

    ///
    /// PUBLIC FUNCTIONS
    ///

    /// A reader for the model's dimensions.
    Dimensions dims() const;

    ///
    /// PUBLIC MEMBER VARIABLES
    ///

    /// A twovec representing endpoints.
    TwoVec endpts;

    /// A twovec representing horizontal connections.
    TwoVec horiz_conns;

    /// A twovec representing vertical connections.
    TwoVec vert_conns;

    /// An integer representing color of path being drawn.
    int orig;

    /// An integer representing number of matched endpoints.
    int completed;

    /// An unordered map representing the status of each color pair.
    std::unordered_map<int, bool> matched;

    /// Chooses a level to load in.
    ge211::Random_source<int> die;

#ifdef CS211_TESTING
    // When this class is compiled for testing, members of a struct named
    // Test_access will be allowed to access private members of this class.
    friend struct Test_access;
#endif

private:
    //
    // PRIVATE MEMBER VARIABLES
    //
    Dimensions dims_;
};





