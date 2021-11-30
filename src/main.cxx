#include "controller.hxx"

int
main(int argc, char *argv[]) {

    try {
        int dim;
        ge211::Random_source<int> die(6, 9);
        switch (argc) {
            case 1:
                dim = die.next();
                break;
            case 2:
                dim = std::stoi(argv[1]);
                break;
            default:
                std::cerr << "Usage: " << argv[0] << " [SIZE]\n";
                return 1;
        }
        if(dim < 6 || dim > 9) {
            std::cerr << "Size not supported. Coming soon.\n";
            return 1;
        }
        Controller(dim).run();
    } catch (std::exception const& e) {
        // Prints out error message if, say, the command-line argument
        // cannot be parsed as `int`s.
        std::cerr << argv[0] << ": " << e.what() << "\n";
        return 1;
    }

    return 0;
}