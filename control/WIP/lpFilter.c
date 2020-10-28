/*
Author: Leslie Nix
Date Created: 27 October 2020
Date Updated: 27 October 2020
Status: Psuedocode/Notes; does not compile

Low Pass Filter bc reasons
*/

/*
Notes and nonsense; not actual code yet
*/

int RawData;
float SmoothData;
float LPF_Beta = 0.025; // 0<ß<1

void main (void){
    // LPF: Y(n) = (1-ß)*Y(n-1) + (ß*X(n))) = Y(n-1) - (ß*(Y(n-1)-X(n)));
    while(1){
       // Function that brings Fresh Data into RawData
       RawData = GetRawData();
       SmoothData = SmoothData - (LPF_Beta * (SmoothData - RawData));
    }
}
