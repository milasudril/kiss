#include <string>
#include <map>
#include <cstdio>

/**This function loads a dictionary from source.
*/
void dictionaryLoad(std::map<std::string,std::string>& dict,FILE* source)
	{
	std::string key;
	std::string buffer;

	int ch_in;

	enum class State:unsigned int{KEY,VALUE,ESCAPE};
	auto state=State::KEY;
	auto state_prev=state;

	while( (ch_in=getc(source))!=EOF )
		{
		if(ch_in=='\r')
			{continue;}

		switch(state)
			{
			case State::KEY:
				switch(ch_in)
					{
					case '_':
						state_prev=state;
						state=State::ESCAPE;
						break;

					case ':':
						key=buffer;
						buffer.clear();
						state=State::VALUE;
						break;

					default:
						buffer+=static_cast<char>( ch_in );
					}
				break;

			case State::VALUE:
				switch(ch_in)
					{
					case '_':
						state_prev=state;
						state=State::ESCAPE;
						break;

					case '\n':
						state=State::KEY;
						dict[key]=buffer;
						buffer.clear();
						break;

					default:
						buffer+=static_cast<char>( ch_in );
					}
				break;

			case State::ESCAPE:
				state=state_prev;
				buffer+=static_cast<char>( ch_in );
				break;
			}
		}
	}

/**This function substitutes variables in source with values from dictionary,
 * and writes the result to dest.
*/
void streamProcess(const std::map<std::string,std::string>& dictionary
	,FILE* source,FILE* dest)
	{
	int ch_in;

	enum class State:unsigned int{TEXT,VARIABLE,ESCAPE};
	auto state=State::TEXT;
	auto state_prev=state;
	std::string varname;

	while( (ch_in=getc(source)) != EOF )
		{
		switch(state)
			{
			case State::TEXT:
				switch(ch_in)
					{
					case '_':
						state=State::ESCAPE;
						break;

					case '$':
						state=State::VARIABLE;
						varname.clear();
						break;

					default:
						putc(ch_in,dest);
					}
				break;

			case State::VARIABLE:
				switch(ch_in)
					{
					case '_':
						state=State::ESCAPE;
						break;

					default:
						if(ch_in>='\0' && ch_in<=' ')
							{
							state=State::TEXT;
							auto i=dictionary.find(varname);
							if(i==dictionary.end())
								{
								fprintf(stderr,"Variable %s not found in the dictionary\n"
									,varname.c_str());
								return;
								}
							fputs(i->second.c_str(),dest);
							}
						else
							{varname+=static_cast<char>(ch_in);}
					}
				break;

			case State::ESCAPE:
				if(state_prev==State::VARIABLE)
					{varname+=static_cast<char>(ch_in);}
				else
					{putc(ch_in,dest);}
				state=state_prev;
				break;
			}
		}
	}

int main(int argc,char* argv[])
	{
	if(argc<2)
		{
		fprintf(stderr,
			"Usage:                                                                       \n"
			"    kiss dictionary ... < file                                               \n"
			"or                                                                           \n"
			"    command | kiss dictionary ...                                            \n"
            "                                                                             \n"
			"Kiss Is a Simple Substitution filter that follows the KISS princlple. `kiss` \n"
			"replaces variables in found in standard input with values from loaded        \n"
			"dictionaries. The result is written to standard output.                      \n"
            "                                                                             \n"
			"Dictionary entries take the form                                             \n"
            "                                                                             \n"
			"    variable:value                                                           \n"
            "                                                                             \n"
			"and are delimited by newline                                                 \n"
            "                                                                             \n"
			"Variables in the input file are delmited by `$` and any whitespace character.\n"
            "                                                                             \n"
			"In both input stream and dictionaries, `_` acts as escape character.         \n");
		return 0;
		}

	std::map<std::string,std::string> dict;
//	Load all dictionaries
	while(argc!=1)
		{
		++argc;
		--argc;
		FILE* source=fopen(*argv,"rb");
		if(source==NULL)
			{
			fprintf(stderr,"Could not load the dictionary %s\n",*argv);
			return -1;
			}

	//	Assume (for now) that dictionaryLoad does not throw any exceptions...
		dictionaryLoad(dict,source);
		fclose(source);
		}

	streamProcess(dict,stdin,stdout);
	return 0;
	}

