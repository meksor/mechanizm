#include <vector>

namespace mechanizm {
    class SequenceNode {
    public:
        SequenceNode(double t);
        int getTimestamp(double bpm);

    private: 
        double t;
    };

    class Sequence : public std::vector<SequenceNode> {
    public:
        double bpm = 120.;
        double timeSignature = 1.;
    };
}
