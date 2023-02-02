#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

#define SCREEN_H 24
#define SCREEN_W 24

const char luminance[12] = {'.', ',', '-', '~', ':', ';', '=', '!', '*', '#', '$', '@'};

const float theta_spacing = 0.07;
const float phi_spacing   = 0.02;

const float R1 = 1;
const float R2 = 2;
const float K2 = 5;
const float K1 = SCREEN_W*K2*3/(8*(R1+R2));

// フレームの描画
void render_frame(float A, float B){
    
    // A, B 三角関数の事前計算で高速化
    float sinA = sin(A), cosA = cos(A);
    float sinB = sin(B), cosB = cos(B);

    // z-bufferとスクリーン
    vector<vector<char>> output(SCREEN_H+1, vector<char>(2*SCREEN_W+1, ' '));
    vector<vector<float>> z_buf(SCREEN_H+1, vector<float>(2*SCREEN_W+1, 0));

    // 断面の中での回転
    for (float theta = 0; theta < 2*M_PI; theta+=theta_spacing) {

        // 事前計算で高速化
        float costheta = cos(theta), sintheta = sin(theta);

        // ドーナツの円環状に回転
        for(float phi=0; phi < 2*M_PI; phi+=phi_spacing) {

            // 事前計算で高速化
            float cosphi = cos(phi), sinphi = sin(phi);

            // 断面の作成
            float circlex = R2 + R1*costheta;
            float circley = R1*sintheta;

            // 作成した断面をドーナツに沿って回転し、空間にプロット
            float x = circlex*(cosB*cosphi + sinA*sinB*sinphi) - circley*cosA*sinB; 
            float y = circlex*(sinB*cosphi - sinA*cosB*sinphi) + circley*cosA*cosB;
            float z = K2 + cosA*circlex*sinphi + circley*sinA;
            float ooz = 1/z;  // "one over z"

            // 二次元に写像
            int xp = (int) (2*(SCREEN_W/2 + K1*ooz*x));
            int yp = (int) (SCREEN_H/2 - K1*ooz*y);

            // 光の計算
            float L = cosphi*costheta*sinB - cosA*costheta*sinphi - sinA*sintheta + cosB*(cosA*sintheta - costheta*sinA*sinphi);

            // 光が指していて、今までで一番近い場合は更新
            if (L > 0) {
                if(ooz > z_buf[yp][xp]) {
                    z_buf[yp][xp] = ooz;
                    int luminance_index = L*8;
                    output[yp][xp] = luminance[luminance_index];
                }
            }

        }

    }
    
    // フレームを書き出し
    printf("\x1b[H\n");
    for(auto line : output){
        for (int _ = 0; _ < 18; _++)printf(" ");  // pandding
        for(char ch : line){
            printf("%c", ch);
        }
        printf("\n");
    }
}

int main(int argc, char const *argv[]){

    float A = 0, B = 0;

    printf("\x1b[2J");

    while (true){
        render_frame(A, B);
        A += 0.004;
        B += 0.002;
    }
    
    return 0;
}