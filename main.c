#include <stdio.h>

typedef struct _pixel {
    unsigned short int red ;
    unsigned short int green ;
    unsigned short int blue;
} Pixel;

typedef struct _image {
    // [width][height][rgb]
    // 0 -> red
    // 1 -> green
    // 2 -> blue
    unsigned short int pixel[512][512][3];
    unsigned int width;
    unsigned int height ;
} Image;

Image LerImagem(Image imagem){
  char p3[4];
  scanf("%s", p3);

  // read width height and color of image
  int max_color;
  scanf("%u %u %d", &imagem.width, &imagem.height , &max_color);

  // read all pixels of image
  for (unsigned int i = 0; i < imagem.height ; ++i) {
      for (unsigned int j = 0; j < imagem.width; ++j) {
          scanf("%hu %hu %hu", &imagem.pixel[i][j][0],
                               &imagem.pixel[i][j][1],
                               &imagem.pixel[i][j][2]);

      }
  }
  return imagem;
}

void ImprimirImagem(Image imagem){

// print type of image
printf("P3\n");
// print width height and color of image
printf("%u %u\n255\n", imagem.width, imagem.height );

// print pixels of image
for (unsigned int i = 0; i < imagem.height ; ++i) {
    for (unsigned int j = 0; j < imagem.width; ++j) {
        printf("%hu %hu %hu ", imagem.pixel[i][j][0],
                               imagem.pixel[i][j][1],
                               imagem.pixel[i][j][2]);
    }
    printf("\n");
  }
  return;
}


Image escala_de_cinza(Image imagem) {
    /*for (unsigned int i = 0; i < img.height ; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            print("%u", img.pixel[i][j][0] + img.pixel[i][j][1] + img.pixel[i][j][2]);
        }
    }*/

    for (unsigned int i = 0; i < imagem.height ; ++i) {
        for (unsigned int j = 0; j < imagem.width; ++j) {
            int media = imagem.pixel[i][j][0] +
                        imagem.pixel[i][j][1] +
                        imagem.pixel[i][j][2];
            media /= 3;
            imagem.pixel[i][j][0] = media;
            imagem.pixel[i][j][1] = media;
            imagem.pixel[i][j][2] = media;
        }
    }

    return imagem;
}

void blur(unsigned int height , unsigned short int pixel[512][512][3], int Tamanho, unsigned int width) {
    for (unsigned int i = 0; i < height ; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            Pixel media = {0, 0, 0};

            int menor_height = (height  - 1 > i + Tamanho/2) ? i + Tamanho/2 : height  - 1;
            int min_width = (width - 1 > j + Tamanho/2) ? j + Tamanho/2 : width - 1;
            for(int x = (0 > i - Tamanho/2 ? 0 : i - Tamanho/2); x <= menor_height; ++x) {
                for(int y = (0 > j - Tamanho/2 ? 0 : j - Tamanho/2); y <= min_width; ++y) {
                    media.red  += pixel[x][y][0];
                    media.green  += pixel[x][y][1];
                    media.blue += pixel[x][y][2];
                }
            }

            // printf("%u", media.red )
            media.red  /= Tamanho * Tamanho;
            media.green  /= Tamanho * Tamanho;
            media.blue /= Tamanho * Tamanho;

            pixel[i][j][0] = media.red ;
            pixel[i][j][1] = media.green ;
            pixel[i][j][2] = media.blue;
        }
    }
}

Image rotacionar90direita(Image imagem) {
    Image rotacionada;

    /*Rotacionando a imagem trocando comprimento por altura e vice versa*/
    rotacionada.width = imagem.height ;
    rotacionada.height  = imagem.width;

    /*For para varrer a imagem inicial e rotacionar*/
    for (unsigned int i = 0, y = 0; i < rotacionada.height ; ++i, ++y) {
        for (int j = rotacionada.width - 1, x = 0; j >= 0; --j, ++x) {
            rotacionada.pixel[i][j][0] = imagem.pixel[x][y][0];
            rotacionada.pixel[i][j][1] = imagem.pixel[x][y][1];
            rotacionada.pixel[i][j][2] = imagem.pixel[x][y][2];
        }
    }

    return rotacionada;
}


void inverter_cores(unsigned short int pixel[512][512][3],
                    unsigned int width, unsigned int height ) {
    for (unsigned int i = 0; i < height ; ++i) {
        for (unsigned int j = 0; j < width; ++j) {
            pixel[i][j][0] = 255 - pixel[i][j][0];
            pixel[i][j][1] = 255 - pixel[i][j][1];
            pixel[i][j][2] = 255 - pixel[i][j][2];
        }
    }
}

Image cortar_imagem(Image imagem, int x, int y, int width, int height ) {
    Image cortada;

    cortada.width = width;
    cortada.height  = height ;

    for(int i = 0; i < height ; ++i) {
        for(int j = 0; j < width; ++j) {
            cortada.pixel[i][j][0] = imagem.pixel[i + y][j + x][0];
            cortada.pixel[i][j][1] = imagem.pixel[i + y][j + x][1];
            cortada.pixel[i][j][2] = imagem.pixel[i + y][j + x][2];
        }
    }

    return cortada;
}


int main() {
    Image img;

    img = LerImagem(img);



    int n_opcoes;
    scanf("%d", &n_opcoes);

    for(int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // Escala de Cinza
                img = escala_de_cinza(img);
                break;
            }
            case 2: { // Filtro Sepia
                for (unsigned int x = 0; x < img.height ; ++x) {
                    for (unsigned int j = 0; j < img.width; ++j) {
                        unsigned short int pixel[3];
                        pixel[0] = img.pixel[x][j][0];
                        pixel[1] = img.pixel[x][j][1];
                        pixel[2] = img.pixel[x][j][2];

                        int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
                        int menor_r = (255 >  p) ? p : 255;
                        img.pixel[x][j][0] = menor_r;

                        p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
                        menor_r = (255 >  p) ? p : 255;
                        img.pixel[x][j][1] = menor_r;

                        p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
                        menor_r = (255 >  p) ? p : 255;
                        img.pixel[x][j][2] = menor_r;
                    }
                }

                break;
            }
            case 3: { // Blur
                int tamanho = 0;
                scanf("%d", &tamanho);
                blur(img.height , img.pixel, tamanho, img.width);
                break;
            }
            case 4: { // Rotacao
                int quantas_vezes = 0;
                scanf("%d", &quantas_vezes);
                quantas_vezes %= 4;
                for (int j = 0; j < quantas_vezes; ++j) {
                    img = rotacionar90direita(img);
                }
                break;
            }
            case 5: { // Espelhamento
                int horizontal = 0;
                scanf("%d", &horizontal);

                int width = img.width, height  = img.height ;

                if (horizontal == 1) width /= 2;
                else height  /= 2;

                for (int i2 = 0; i2 < height ; ++i2) {
                    for (int j = 0; j < width; ++j) {
                        int x = i2, y = j;

                        if (horizontal == 1) y = img.width - 1 - j;
                        else x = img.height  - 1 - i2;

                        Pixel aux1;
                        aux1.red  = img.pixel[i2][j][0];
                        aux1.green  = img.pixel[i2][j][1];
                        aux1.blue = img.pixel[i2][j][2];

                        img.pixel[i2][j][0] = img.pixel[x][y][0];
                        img.pixel[i2][j][1] = img.pixel[x][y][1];
                        img.pixel[i2][j][2] = img.pixel[x][y][2];

                        img.pixel[x][y][0] = aux1.red ;
                        img.pixel[x][y][1] = aux1.green ;
                        img.pixel[x][y][2] = aux1.blue;
                    }
                }
                break;
            }
            case 6: { // Inversao de Cores
                inverter_cores(img.pixel, img.width, img.height );
                break;
            }
            case 7: { // Cortar Imagem
                int x, y;
                scanf("%d %d", &x, &y);
                int width, height ;
                scanf("%d %d", &width, &height );

                img = cortar_imagem(img, x, y, width, height );
                break;
            }
        }

    }

    ImprimirImagem(img);




    return 0;
}
