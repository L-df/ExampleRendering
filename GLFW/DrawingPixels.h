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

    constexpr Vector2(NumericType InX, NumericType InY) { this->X = InX; this->Y = InY; };
    constexpr Vector2() : X(0), Y(0) { };
};

struct Sphere
{
    union
    {
        Vector2<int> Center;
        int q[2];
    };
    union 
    {
        Vector2<int> Velocity;
        int p[2];
    };
    int Radius;

    constexpr Sphere() : Center(), Velocity(), Radius(0) { };
};


template<int Width, int Height>
struct SimplePixelArray
{
public:
    GLFWPixel PixelData[Width*Height] = {};

    constexpr GLFWPixel& GetPixel(int x, int y) { return PixelData[y*Width + x]; };
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

constexpr Sphere CreateSphere(Vector2<int> Position, int Radius, Vector2<int> Velocity)
{
    Sphere ReturnObject = Sphere();

    ReturnObject.Center.X = Position.X;
    ReturnObject.Center.Y = Position.Y;
    ReturnObject.Radius = Radius;

    ReturnObject.Velocity.X = Velocity.X;
    ReturnObject.Velocity.Y = Velocity.Y;
    
   return ReturnObject;
}

template<typename NumericType>
constexpr NumericType Square(NumericType X)
{
    NumericType ReturnVal = X * X;
    return ReturnVal;
}

template<int Width, int Height>
constexpr SimplePixelArray<Width, Height> 
    GenerateSimpleColor(
    const int& r, const int& g, const int& b,
    const SimplePixelArray<Width, Height>& BaseImageToChange, 
    const Sphere& SphereToDraw)
{
    SimplePixelArray<Width, Height> ReturnArray = BaseImageToChange;

    int RadiusSquared = Square(SphereToDraw.Radius);    

    for(int HeightIndex = 0; HeightIndex < Height; HeightIndex++)
    {
        int HeightDifference = Square(HeightIndex - SphereToDraw.Center.Y);
        
        for(int WidthIndex = 0; WidthIndex < Width; WidthIndex++)
        {
            int WidthDifference = Square(WidthIndex - SphereToDraw.Center.X);
            if(HeightDifference + WidthDifference < RadiusSquared)
            {
                ReturnArray.GetPixel(WidthIndex, HeightIndex).r = r;
                ReturnArray.GetPixel(WidthIndex, HeightIndex).g = g;
                ReturnArray.GetPixel(WidthIndex, HeightIndex).b = b;
            }
        }
    }

    return ReturnArray;
}




