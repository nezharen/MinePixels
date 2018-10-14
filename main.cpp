#include <QImage>
#include <cstdio>
#define COLOR_TYPES 16

int color[COLOR_TYPES] = {0x8b8f97, 0x984200, 0x722070, 0x185d8e,
                          0xa87a0f, 0x437614, 0x944567, 0x25272c,
                          0x575754, 0x0f5364, 0x461672, 0x1f2169,
                          0x422917, 0x323e1a, 0x5f1516, 0x06080c};
bool b[COLOR_TYPES];
char a[1000][1000];
char c[COLOR_TYPES];

inline int R(const int &p)
{
    return color[p] >> 16;
}

inline int G(const int &p)
{
    return (color[p] >> 8) & 0xff;
}

inline int B(const int &p)
{
    return color[p] & 0xff;
}

inline int square(const int& x)
{
    return x * x;
}

int calcDis(const QRgb& color, const int& p)
{
    return square(qRed(color) - R(p)) + square(qGreen(color) - G(p)) + square(qBlue(color) - B(p));
}

int main(int argc, char* argv[])
{
    memset(b, false, sizeof(b));
    QImage image = QImage(argv[1]);
    for (int i = 0; i < image.width(); i++)
        for (int j = 0; j < image.height(); j++)
        {
            QRgb t = image.pixel(i, j);
            int min_dis = INT_MAX;
            int p = -1;
            for (int k = 0; k < COLOR_TYPES; k++)
            {
                int tdis = calcDis(t, k);
                if (tdis < min_dis)
                {
                    min_dis = tdis;
                    p = k;
                }
            }
            image.setPixel(i, j, qRgb(R(p), G(p), B(p)));
            a[i][j] = p;
            b[p] = true;
        }
    image.save("result.png");

    FILE* f = fopen("result.txt", "w");
    fprintf(f, "Color maps:\n");
    int sum = 0;
    for (int i = 0; i < COLOR_TYPES; i++)
        if (b[i])
        {
            c[i] = sum;
            fprintf(f, "%x: %x\n", sum, i);
            sum++;
        }
    fprintf(f, "\nPixels:\n");

    for (int j = 0; j < image.height(); j++)
    {
        for (int i = 0; i < image.width(); i++)
        {
            if (i > 0 && i % 10 == 0)
                fprintf(f, " ");
            fprintf(f, "%x", c[a[i][j]]);
        }
        fprintf(f, "\n");
    }
    fclose(f);

    return 0;
}
