#include<stdio.h>
#include <algorithm>
#include <vector>

int black_value = 0;
int grey_value = 0;
int white_value = 0;

int black_grey_thresh = 0;
int white_grey_thresh = 0;


const int max_value = 900;
const int array_length = 120;

int array[array_length] = { 0 };
std::vector<int> values;

void find_max(){
    int localMax[array_length/2] = {0};
    int pointerToCurrentLocalMax = 0;

    //save all local maxima in localMax[]
    for (int i = 1; i < array_length; i++){
        if (array[i-1]< array[i] && array[i]>array[i+1]){
            localMax[pointerToCurrentLocalMax] = i;
            pointerToCurrentLocalMax ++;
        }
    }

    int max1 = 0, max2 = 0, max3 = 0;
    for (int i = 0; i < array_length/2; i++){
        if (array[localMax[i]] > array[max1]){
            max3 = max2;
            max2 = max1;
            max1 = localMax[i];
        }else if (array[localMax[i]] > array[max2]){
            max3 = max2;
            max2 = localMax[i];
        }else if (array[localMax[i]] > array[max3]){
            max3 = localMax[i];
        }
    }

    black_value = std::min(max1, std::min(max2, max3));
    white_value = std::max(max1, std::max(max2, max3));
    grey_value = max1 + max2 + max3 - black_value - white_value;

    //compute thresholds
    black_grey_thresh = (int) ((black_value + grey_value)/ 2);
    white_grey_thresh = (int) ((white_value + grey_value)/ 2);
}

void addValueToBuckets(int value){
    array[value*array_length/max_value]++;
}

int predict(int* data, int num_prev_data){
    //your new data should be data[num_prev_data-1];
    int value = data[num_prev_data-1];
    /*values.push_back(value);
    if(values.size() > 110)
        values.erase(values.begin());
    for (int i = 0; i < array_length; ++i){
        array[i] = 0;
    }

    for(int i = 0; i < values.size(); i++){
        addValueToBuckets(values.at(i));
    }

    find_max();*/
    if(num_prev_data == 1){
        for (int i = 0; i < array_length; ++i)
        {
            array[i] = 0;
        }
    }
    addValueToBuckets(data[num_prev_data-1]);
    find_max();
    //output: 0 for black, 1 for gray and 2 for white.
    if(value*array_length/max_value < black_grey_thresh)return 0;
    if(value*array_length/max_value < white_grey_thresh)return 1;
    else return 2;
}


//////////////////////////////////////////////////////////
//////////////////DO NOT MODIFY BELOW/////////////////////
//////////////////////////////////////////////////////////
int main(void){
    float average_score = 0;
    for(int f=1; f <= 4; f++){
        char filename[10];
        sprintf(filename, "data%d.txt", f);
        FILE* fp = fopen(filename, "r");

        int N;
        fscanf(fp, "%d", &N);
        int data[N];
        int score = 0;
        for(int i=0; i < N; i++){
            int d, label;
            fscanf(fp, "%d %d", &d, &label);
            data[i] = d;
            int pred = predict(data, i+1);
            if(pred == label)score++;
        }
        printf("round %d: your score: %f\n",f, score / float(N));
        average_score += score/float(N);
        //score the prediction
        fclose(fp);
    }
    average_score /= 4;
    printf("average score: %f\n", average_score);
}