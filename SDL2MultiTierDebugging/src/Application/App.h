#include <stdlib.h>

#include <SDL.h>
#include <GLES2/gl2.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

namespace Application
{
    class App
    {
    public:
        int Run();
       
    protected:
        GLuint LoadShader(const char *shaderSrc, GLenum type);
       
        int init();
        
        void display();
       
    private:
        int32_t m_shaderProgram;
        float m_width;
        float m_height;  
    
    private:
        static const int32_t POSITION_PARAMETER_INDEX /*= 0*/;
        
        static const int32_t COLOR_PARAMETER_INDEX /*= 1*/;
    };
}