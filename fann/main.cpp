#include <iostream>
#include <fann.h>


int main( int argc, char* argv[] )
{
    if (argc < 2){

        const unsigned int num_input = 10;
        const unsigned int num_output = 3;
        const unsigned int num_layers = 4;
        const unsigned int num_neurons_hidden = 8;
        const float desired_error = (const float) 0.0000001;
        const unsigned int max_epochs = 900000;
        const unsigned int epochs_between_reports = 10000 ;

        struct fann *ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_neurons_hidden-3, num_output);

        fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
        fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

        fann_train_on_file(ann, "imiona.data", max_epochs, epochs_between_reports, desired_error);

        fann_save(ann, "imiona.net");

        fann_destroy(ann);
    }
    else {
        std::cout << "\n \ntest" <<std::endl;

        fann_type *calc_out;
        fann_type input[10];

        for (int i = 0 ; i <10;++i){
            input[i] = 0;
        }
        struct fann *ann2 = fann_create_from_file("imiona.net");
        std::string imie = argv[1];

        for (int i = 0 ; i < imie.size(); ++i){
            input[i] = (float)(imie.at(i))/1000;
        }

        for (int i = 0 ; i <10;++i){
            std::cout << input[i] <<" "  ;
        }
        std::cout << " " << std::endl;
        calc_out = fann_run(ann2, input);

        printf("kobieta %f | facet%f | inne %f)   \n ",   calc_out[0],calc_out[1],calc_out[2]);

        printf("\n");
        // fann_print_connections(ann2);
        fann_destroy(ann2);
    }
    return 0;
}


