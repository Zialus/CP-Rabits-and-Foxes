#include <iostream>
#include <vector>

int GEN_PROC_RABBITS ; // number of generations until a rabbit can procreate
int GEN_PROC_FOXES ; // number of generations until a fox can procreate
int GEN_FOOD_FOXES ; // number of generations for a fox to die of starvation
int N_GEN ; // number of generations for the simulation
int R ; // number of rows of the matrix representing the ecosystem
int C ; // number of columns of the matrix representing the ecosystem
int N ; // number of objects in the initial ecosystem

typedef enum { RABBIT, FOX, ROCK, EMPTY } ElementType;

struct Rock {

    Rock(){}

    Rock(int x,int y){
        pos_x = x;
        pos_y = y;
    }

    int pos_x;
    int pos_y;
};

struct Rabbit {

    Rabbit(){}

    Rabbit(int a, int x, int y) {
        procAge = a;
        pos_x = x;
        pos_y = y;
    }

    int procAge;
    int pos_x;
    int pos_y;
};

struct Fox {

    Fox(){}

    Fox(int a, int x, int y) {
        hungryAge = 0;
        procAge = a;
        pos_x = x;
        pos_y = y;
    }

    int hungryAge;
    int procAge;
    int pos_x;
    int pos_y;
};


struct MatrixElement{

    MatrixElement(){}

    MatrixElement(ElementType type){
        element_type = type;
    }

    ElementType element_type;

    union {
        struct Rabbit rb;
        struct Fox fx;
        struct Rock rk;
    } e;

    int ID;

    char getChar(){
        switch (this->element_type){
            case RABBIT: return 'R'; break;
            case FOX: return 'F'; break;
            case ROCK: return '*'; break;
            case EMPTY: return ' '; break;
        }
    }
};


void printFinalResults(MatrixElement** matrix, int R, int C, int GEN_PROC_RABBITS, int GEN_PROC_FOXES, int GEN_FOOD_FOXES){

    int counter = 0;

    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            if (matrix[i][j].element_type == ElementType::RABBIT) {
                counter++;
            } else if (matrix[i][j].element_type == ElementType::FOX) {
                counter++;
            } else if (matrix[i][j].element_type == ElementType::ROCK) {
                counter++;
            }
        }
    }

    printf("%d %d %d %d %d %d %d\n", GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES, 0, R, C, counter);

    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++){
            if (matrix[i][j].element_type == ElementType::RABBIT) {
                printf("RABBIT %d %d\n", i, j);
            } else if (matrix[i][j].element_type == ElementType::FOX) {
                printf("FOX %d %d\n", i, j);
            } else if (matrix[i][j].element_type == ElementType::ROCK) {
                printf("ROCK %d %d\n", i, j);
            }
        }
    }

}

void printMatrix(MatrixElement** matrix, int R, int C) {

    for (int i = 0; i < R+2; ++i) {
        printf("-");
    }
    printf("\n");

    for (int i = 0; i < R; ++i) {
        printf("|");
        for (int j = 0; j < C; ++j) {
            printf("%c", matrix[i][j].getChar());
        }
        printf("|\n");
    }

    for (int i = 0; i < R+2; ++i) {
        printf("-");
    }
    printf("\n");

}

void simGen(){


}


std::vector<char> checkAdjacencies(MatrixElement** matrix ,int X, int Y){
    std::vector<char> validPositions;
    if(X-1 > 0 && matrix[X-1][Y].element_type == ElementType::EMPTY){ //NORTH
        validPositions.push_back('N');
    }

    if(Y+1 < C && matrix[X][Y+1].element_type == ElementType::EMPTY){ //EAST
        validPositions.push_back('E');
    }

    if(X+1 < R && matrix[X+1][Y].element_type == ElementType::EMPTY){ // SOUTH
        validPositions.push_back('S');
    }

    if(Y-1 > 0 && matrix[X][Y-1].element_type == ElementType::EMPTY) { //WEST
        validPositions.push_back('W');
    }


    return validPositions;
}

std::pair<int,int> chooseMovePosition(int currentGen, int xPos, int yPos, std::vector<char> posVal){
    int pos = (int) ((currentGen + xPos + yPos) % posVal.size());

    if(posVal[pos] == 'N'){
        std::pair<int, int> posPair = std::make_pair(xPos-1,yPos);
        return posPair;
    }

    else if(posVal[pos] == 'E'){
        std::pair<int, int> posPair = std::make_pair (xPos,yPos+1);
        return posPair;
    }

    else if(posVal[pos] == 'S'){
        std::pair<int, int> posPair = std::make_pair (xPos+1,yPos);
        return posPair;
    }

    else if(posVal[pos] == 'W'){
        std::pair<int, int> posPair = std::make_pair (xPos,yPos-1);
        return posPair;
    }

    perror("Function was used improperly");
    exit(1);

};

void analyzeRabbits(MatrixElement** matrix, std::vector<Rabbit> rabbitList, int currentGen){
    std::vector<Rabbit> rabbitsListTemp;
    while(!rabbitList.empty()){
        Rabbit r = rabbitList.front();
        rabbitList.erase(rabbitList.begin());
        int x = r.pos_x;
        int y = r.pos_y;
        std::vector<char> validPositions = checkAdjacencies(matrix, x, y);
        if(validPositions.size() > 0){
            std::pair<int,int> posToMove = chooseMovePosition(currentGen,x,y,validPositions);
            Rabbit rabbitTemp = Rabbit(r.procAge+1, posToMove.first , posToMove.second);


            rabbitsListTemp.push_back(rabbitTemp);

            //to do: add to posmatrixTemp
        }
        else{
            Rabbit rabbitTemp = Rabbit(r.procAge+1, x, y);
            rabbitsListTemp.push_back(rabbitTemp);
        }
    }
}

int main(int argc, char* argv[]) {

    MatrixElement** posMatrix;
    MatrixElement** posMatrixTemp;

    std::vector<Rabbit> RabbitsList;
    std::vector<Fox> FoxesList;

    if (argc == 2){
        freopen(argv[1], "r", stdin);
    }

    std::cin >> GEN_PROC_RABBITS >> GEN_PROC_FOXES >> GEN_FOOD_FOXES >> N_GEN >> R >> C >> N;

    posMatrix = new MatrixElement*[R];
    posMatrixTemp = new MatrixElement*[R];

    for (int i = 0; i < R; ++i) {
        posMatrix[i] = new MatrixElement[C];
        posMatrixTemp[i] = new MatrixElement[C];
    }


    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            MatrixElement el = MatrixElement(ElementType::EMPTY);

            posMatrix[i][j] = el;
            posMatrixTemp[i][j] = el;
        }
    }

    for (int i = 0; i < N; ++i) {
        std::string TYPE;
        int X;
        int Y;

        std::cin >> TYPE >> X >> Y;

        if (TYPE == "RABBIT"){
            Rabbit r = Rabbit(0, X, Y);
            RabbitsList.push_back(r);
            MatrixElement el = MatrixElement(ElementType::RABBIT);
            el.e.rb = r;
            posMatrix[X][Y] = el;
        }
        else if (TYPE == "FOX"){
            Fox f = Fox(0,X,Y);
            FoxesList.push_back(f);
            MatrixElement el = MatrixElement(ElementType::FOX);
            el.e.fx = f;
            posMatrix[X][Y] = el;
        }
        else if (TYPE == "ROCK"){
            MatrixElement el = MatrixElement(ElementType::ROCK);
            posMatrix[X][Y] = el;
        }
        else {
            std::cout << "YOU FUCKED UP" << std::endl;
        }
    }

    for(int i=0; i<N_GEN; i++){
        simGen();
    }

    printMatrix(posMatrix, R, C);

    printFinalResults(posMatrix, R, C, GEN_PROC_RABBITS, GEN_PROC_FOXES, GEN_FOOD_FOXES);

    return 0;
}
