/*int a = 5;
double b = 4;

*
 *
 *  Multi
 *  line
 *  comments
 *  moments
 *
 *
 *
 *
*/ /*
class Mlass
{
public:
  //  int a = 8;
  //  double b = 5;
    int func()
    {
        int val = 9;
    }
protected:
  //  int c = 7;
    int function(int a)
    {
        // doing nothing
    }
};*/
/*
// coment moment
struct Mruct
{
    int a;
    unsigned b;
    char    *aa;
    int     *arr;
    Mruct(int val)
    {
        a = val;
    }
private:
    short   c;
    void    munction(int a, int b)
    {
        int cc = 8;
        b = 8;
        float f = 8.5;
    }
};
*/

void displayNum(int n1, float n2) {
    cout << "The int number is " << n1;
    cout << "The double number is " << n2;
}

int add(int a, int b) {
   return (a + b);
}

int max(int num1, int num2) {
   // local variable declaration
   int result;
 
   if (num1 > num2)
      result = num1;
   else
      result = num2;
 
   return result; 
}

int main(int argc, char *argv[])
{
    int a;
    int b;
    Mlass ob;
    Mruct mob(6);
}

eof
