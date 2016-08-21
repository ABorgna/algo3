#include <stdint.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

using namespace std;


string Solutions(int b [],string st,bool f)
{
    
    string vacio = " ";
    //Death esta para sacarse de encima cuando los can se comen a los arq
    //fruta esta para sacarse de encima los casos como el siguiente:
    //Esat la linterna al inicio y hay un solo arq y la recurstion trata de mandar 2 arq
    //Eso lo hace viendo que el array no tenga numeros negativos
    bool fruta = b[0]<0 || b[1]<0 || b[2]<0 || b[3]<0;
    bool death = (b[1]>b[0] && b[0]>0) || (b[3]>b[2] && b[2]>0);
    if (fruta || death)
    {   
        return vacio ;
    }

    //  El f dice donde esta la linterna , true si esta al inicio y falste al final 
        if(f) 
         {  //Hay que mandar la linterna al otro lado junto con al menos 1 persona
            //Defino los arrays para las recursiones 
            // Arq al inicio , can al inicio, arq al final , can al final         
            //Del puente
            int  A [4] = {b[0]-1,b[1]+0,b[2]+1,b[3]+0};
            int  C [4] = {b[0]+0,b[1]-1,b[2]+0,b[3]+1};
            int AA [4] = {b[0]-2,b[1]+0,b[2]+2,b[3]+0};
            int CC [4] = {b[0]+0,b[1]-2,b[2]+0,b[3]+2};
            int AC [4] = {b[0]-1,b[1]-1,b[2]+1,b[3]+1};
            return Solutions(A,st+"-A",!f) + Solutions(C,st+"-C",!f) + Solutions(AA,st+"-AA",!f) \
            + Solutions(CC,st +"-CC",!f) + Solutions(AC,st+"-AC",!f); 
         }
    else
    // La linterna esta al final 
    {  
        //Revisa si en el inicio no hay nadie, en ese caso imprime el string
        if( b[0]==0 && b[1]==0 )
            {
                string res = "\n Una solucion posible es " + st;
                return res;
            }
        else
        {   //Hay que mandar la linterna con al menos 1 persona
            //Defino los arrays para las recursiones
            // Arq al inicio , can al inicio, arq al final , can al final         
            //Del puente
            int  A [4] = {b[0]+1,b[1]+0,b[2]-1,b[3]+0};
            int  C [4] = {b[0]+0,b[1]+1,b[2]+0,b[3]-1};
            int AA [4] = {b[0]+2,b[1]+0,b[2]-2,b[3]+0};
            int CC [4] = {b[0]+0,b[1]+2,b[2]+0,b[3]-2};
            int AC [4] = {b[0]+1,b[1]+1,b[2]-1,b[3]-1};
            return Solutions(A,st+"-A",!f) + Solutions(C,st+"-C",!f) + Solutions(AA,st+"-AA",!f) \
            + Solutions(CC,st +"-CC",!f) + Solutions(AC,st+"-AC",!f); 
        }

   }

}   


int main() {
    int cantArq;
    int cantCan;
    cout << "Ingrese cantidad de arquelogost y canibalest" << endl;
    cin >> cantArq >> cantCan;
    int A [4] = {cantArq,cantCan,0,0};
    //El array es Arq al inicio del puente, can al inicio del puente, arq al final del puente
    //y can al final del puente
    string empty =" ";
    string st = Solutions(A,empty,true); 
    cout << st << endl;
    //cout << t << endl;

    return 0;
}
