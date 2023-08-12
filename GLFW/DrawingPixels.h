#include <math.h>

struct GLFWPixel
{
public:
    unsigned char r = 0;
    unsigned char g = 0;
    unsigned char b = 0;
};

template<typename NumericType = int>
struct Vector2
{
    NumericType X;
    NumericType Y;
};

struct Sphere
{
    union
    {
        Vector2<int> Center;
        int C[2];
    };
    int Radius;
};


template<int Width, int Height>
struct SimplePixelArray
{
public:
    GLFWPixel PixelData[Width*Height] = {};

    constexpr GLFWPixel& GetPixel(int x, int y) { return PixelData[x*Width + y]; };
};

template<int Width, int Height>
constexpr SimplePixelArray<Width, Height> GenerateSimpleColor(int r, int g, int b)
{
    SimplePixelArray<Width, Height> ReturnArray;

    for(int index = 0; index < Width * Height; index++)
    {
        ReturnArray.PixelData[index].r = r;
        ReturnArray.PixelData[index].g = g;
        ReturnArray.PixelData[index].b = b;
    }

    return ReturnArray;
}


constexpr Sphere CreateSphere(int CenterX, int CenterY, int Radius)
{
    Sphere ReturnObject = Sphere();

    ReturnObject.Center.X = CenterX;
    ReturnObject.Center.Y = CenterY;
    ReturnObject.Radius = Radius;
    
   return ReturnObject;
}

template<int Width, int Height>
constexpr SimplePixelArray<Width, Height> 
    GenerateSimpleColor(
    const int& r, const int& g, const int& b,
    const SimplePixelArray<Width, Height>& BaseImageToChange, 
    const Sphere& SphereToDraw)
{
    SimplePixelArray<Width, Height> ReturnArray = BaseImageToChange;

    for(int HeightIndex = 0; HeightIndex < Height; HeightIndex++)
    {
        int HeightDifference = std::abs(HeightIndex - SphereToDraw.Center.Y);
        
        for(int WidthIndex = 0; WidthIndex < Width; WidthIndex++)
        {
            int WidthDifference = std::abs(WidthIndex - SphereToDraw.Center.X);
            if(HeightDifference + WidthDifference < SphereToDraw.Radius)
            {
                ReturnArray.GetPixel(WidthIndex, HeightIndex).r = r;
                ReturnArray.GetPixel(WidthIndex, HeightIndex).g = g;
                ReturnArray.GetPixel(WidthIndex, HeightIndex).b = b;
            }
        }
    }

    return ReturnArray;
}




