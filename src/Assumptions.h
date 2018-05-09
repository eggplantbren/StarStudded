#ifndef StarStudded_Assumptions_h
#define StarStudded_Assumptions_h

namespace StarStudded
{

class Assumptions
{
    // A static instance
    private:
        static Assumptions instance;
    public:
        static Assumptions& get_instance();

    private:
        // Member variables
        unsigned int max_num_stars;
        double padding;

    public:
        // Load from a YAML file
        void load(const char* setup_filename);
};

} // namespace StarStudded

#endif

