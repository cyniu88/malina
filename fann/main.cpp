#include <iostream>
#include <fann.h>


int main( int argc, char* argv[] )
{
//    std::cout << "argument argc: " << argc << " pierwszy: "<< argv[1] << " drugi: " << argv[2] <<std::endl;
//    const unsigned int num_input = 4;
//    const unsigned int num_output = 1;
//    const unsigned int num_layers = 3;
//    const unsigned int num_neurons_hidden = 4;
//    const float desired_error = (const float) 0.0000001;
//    const unsigned int max_epochs = 500;
//    const unsigned int epochs_between_reports = 1 ;

//    struct fann *ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output);

//    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
//    fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

//    fann_train_on_file(ann, "fann.data", max_epochs, epochs_between_reports, desired_error);

//    fann_save(ann, "fann.net");

//    fann_destroy(ann);
std::cout << "\n \ntest" <<std::endl;

    fann_type *calc_out;
    fann_type input[4];

    struct fann *ann2 = fann_create_from_file("fann.net");

    input[0] = std::stof(argv[1]);
    input[1] = std::stof(argv[2]);
    input[2] = std::stof(argv[3]);
    input[3] = std::stof(argv[4]);
    calc_out = fann_run(ann2, input);

    printf("xor test (%f,%f) -> %f\n", input[0], input[1], calc_out[0]);
    if (calc_out[0] > 0.5){
        printf("play golf");
    }
    else {
        printf("do not plaay golf!");
    }
    printf("\n");
    fann_destroy(ann2);
    return 0;
}


