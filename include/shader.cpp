#include "shader.h"
#include <include_gl.h>

void shader::loadFile(const char* fn,std::string& str)
{
	std::ifstream in(fn);
	if(!in.is_open())
	{
		std::cout << "The file " << fn << " cannot be opened\n";
		return;
	}
	char tmp[300];
	while(!in.eof())
	{
		in.getline(tmp,300);
		str+=tmp;
		str+='\n';
	}
}

unsigned int shader::loadShader(std::string& source,unsigned int mode)
{
    unsigned int id;
    id=glCreateShader(mode);
	
	const char* csource=source.c_str();
	
    glShaderSource(id,1,&csource,NULL);
    glCompileShader(id);
    char error[1024]={};
    glGetShaderInfoLog(id,1000,NULL,error);
	std::cout << "Compile status: \n";
	
	if (strlen(error))
		std::cout << "{" << source << "}" << std::endl;

	std::cout << error << std::endl;
	return id;
}

shader::shader(std::string vss, std::string fss)
{
	std::string source;
    loadFile(vss.c_str(),source);
#ifndef ANDROID
    source = "#define mediump\n#define lowp\n#define highp\n" + source;
#endif
	vs=loadShader(source,GL_VERTEX_SHADER);
	source="";
    loadFile(fss.c_str(),source);
#ifndef ANDROID
    source = "#define mediump\n#define lowp\n#define highp\n" + source;
#endif
	fs=loadShader(source,GL_FRAGMENT_SHADER);
	
    program=glCreateProgram();
    glAttachShader(program,vs);
    glAttachShader(program,fs);
	
    glLinkProgram(program);

    char errBuffer[1024]={};
    glGetProgramInfoLog(program, sizeof(errBuffer), 0, errBuffer);

    glUseProgram(program);
}

shader::~shader()
{
    glDetachShader(program,vs);
    glDetachShader(program,fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteProgram(program);
}

void shader::use()
{
    glUseProgram(program);
}

unsigned int shader::getProgramId()
{
	return program;
}


void shader::release()
{
    glUseProgram(0);
}
		
