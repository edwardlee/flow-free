#include "model.hxx"
#include <catch.hxx>

TEST_CASE("Reset")
{
    Model m(6);

    /// Fill the model with connections.
    m.vert_conns[0][0] = 0xBEEF;
    m.horiz_conns[3][3] = 0xCAFE;
    m.horiz_conns[0][0] = 0xF00D;
    m.vert_conns[0][4] = 0xFACE;

    /// Make a copy of the model.
    Model m2(6);

    /// Fill the copy with the same connections.
    m2.vert_conns[0][0] = 0xBEEF;
    m2.horiz_conns[3][3] = 0xCAFE;
    m2.horiz_conns[0][0] = 0xF00D;
    m2.vert_conns[0][4] = 0xFACE;

    /// Manually reset the copy.
    for(auto& row : m2.horiz_conns)
        for(auto& e : row)
            e = 0;
    for(auto& row : m2.vert_conns)
        for(auto& e : row)
            e = 0;

    /// Call reset on the main model and see if the two models are equivalent.
    m.reset();
    CHECK(m.horiz_conns == m2.horiz_conns);
    CHECK(m.vert_conns == m2.vert_conns);
}

TEST_CASE("Cleanup")
{
    Model m(6);

    /// Populate model connections.
    m.vert_conns[0][0] = 1;
    m.vert_conns[4][4] = 1;
    m.horiz_conns[0][0] = 1;
    m.horiz_conns[4][1] = 2;

    m.cleanup(1);

    /// Check all conns with val 1 are cleaned.
    CHECK(m.vert_conns[0][0] == 0);
    CHECK(m.vert_conns[4][4] == 0);
    CHECK(m.horiz_conns[0][0] == 0);

    /// Conns not harboring the target value are left unharmed.
    CHECK(m.horiz_conns[4][1] == 2);
}

TEST_CASE("Overlap")
{
    Model m(6);

    /// Create a red L shape path on the board.
    m.vert_conns[0][1] = 1;
    m.vert_conns[1][1] = 1;
    m.vert_conns[2][2] = 1;
    m.horiz_conns[2][2] = 1;
    m.horiz_conns[2][3] = 2;

    /// Simulate player drawing thru (1,1).
    /// False indicates swiping in horizontal direction.
    m.overlap({1, 1}, false);

    /// Behavior is like cleanup.
    CHECK(m.vert_conns[0][1] == 0);
    CHECK(m.vert_conns[1][1] == 0);
    CHECK(m.vert_conns[2][2] == 0);
    CHECK(m.horiz_conns[2][2] == 0);

    /// Blue value is untouched.
    CHECK(m.horiz_conns[2][3] == 2);
}

TEST_CASE("Instantiate")
{
    Model m(6);

    /// Force a given level via instantiate.
    m.die.stub_with(0);
    m.instantiate();

    /// Make a copy of the model.
    Model m2(6);

    /// Populate the copy with what level should have been loaded in.
    m2.endpts = {
        {0, 0, 0, 0, 0, 5},
        {0, 0, 0, 0, 0, 0},
        {0, 4, 1, 0, 0, 0},
        {0, 0, 0, 3, 0, 4},
        {0, 3, 0, 5, 1, 2},
        {0, 0, 0, 2, 0, 0}
    };

    CHECK(m.endpts == m2.endpts);
}

TEST_CASE("Solved")
{
    Model m(6);

    m.matched[1] = true;
    m.matched[2] = true;
    m.matched[3] = true;
    m.matched[4] = true;
    m.matched[5] = true;
    m.matched[6] = true;
    m.matched[7] = true;

    /// All colors are matched. Puzzle is solved.
    CHECK(m.solved());

    /// Solved ignores invalid color indices.
    m.matched[-1] = false;
    CHECK(m.solved());

    /// 6 maps to teal, so setting it as disconnected (false) makes puzzle unsolved.
    m.matched[6] = false;
    CHECK_FALSE(m.solved());
}
