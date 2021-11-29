#pragma once

#include "board.hxx"

#include <ge211.hxx>

#include <iostream>
#include <vector>

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

    /// A reader for the endpoints twovec.
    TwoVec endpts() const;

    /// A reader for the horizontal connections twovec.
    TwoVec horiz_conns() const;

    /// A reader for the vertical connections twovec.
    TwoVec vert_conns() const;

    /// Places a connection in the valid connections vector.
    void place_conn(Position p);

    /// A twovec representing endpoints.
    TwoVec endpts_;

    /// A twovec representing horizontal connections.
    TwoVec horiz_conns_;

    /// A twovec representing vertical connections.
    TwoVec vert_conns_;

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





