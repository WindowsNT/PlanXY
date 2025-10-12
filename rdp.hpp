
namespace RDP {


	namespace MIME2
	{
		class CONTENT;
		class CONTENTBUILDER;
		using namespace std;
		void BuildZ(CONTENT& c1, CONTENT& c2, CONTENT& co, const char* s);


		enum class MIMEERR
		{
			OK = 0,
			INVALID = 1,
			NOTSIGNED = 2,
			ERRSIGN = 3,
		};



		inline string Char2Base64(const char* Z, size_t s)
		{
			DWORD da = 0;
			CryptBinaryToString((const BYTE*)Z, (DWORD)s, CRYPT_STRING_BASE64, 0, &da);
			da += 100;
			unique_ptr<char> out(new char[da]);
			CryptBinaryToStringA((const BYTE*)Z, (DWORD)s, CRYPT_STRING_BASE64, out.get(), &da);
			return out.get();
		}

		inline void Base64ToChar(const char* Z, size_t s, vector<char>& out)
		{
			DWORD dw = 0;
			CryptStringToBinaryA(Z, (DWORD)s, CRYPT_STRING_BASE64, 0, &dw, 0, 0);
			out.resize(dw);
			CryptStringToBinaryA(Z, (DWORD)s, CRYPT_STRING_BASE64, (BYTE*)out.data(), &dw, 0, 0);
		}


		MIMEERR ParseMultipleContent2(const char* d, size_t sz, const char* del, vector<CONTENT>& Result);

		inline void Split(const char* m, char del, vector<string>& result)
		{
			if (!m)
				return;
			stringstream ss(m);
			while (ss.good())
			{
				string substr;
				std::getline(ss, substr, del);
				result.push_back(substr);
			}
		}

		inline string& Trim(string& s, int j = 0)
		{
			while (s.length() && (j == 0 || j == 1))
			{
				if (s[s.length() - 1] == ' ' || s[s.length() - 1] == '\r' || s[s.length() - 1] == '\n' || s[s.length() - 1] == '\t')
					s.erase(s.end() - 1);
				else
					break;
			}
			while (s.length() && (j == 0 || j == 2))
			{
				if (s[0] == ' ' || s[0] == '\r' || s[0] == '\n' || s[0] == '\t')
					s.erase(s.begin());
				else
					break;
			}
			return s;
		}

		inline vector<char>& Trim(vector<char>& s, int j = 0)
		{
			while (s.size() && (j == 0 || j == 1))
			{
				if (s[s.size() - 1] == ' ' || s[s.size() - 1] == '\r' || s[s.size() - 1] == '\n' || s[s.size() - 1] == '\t')
					s.erase(s.end() - 1);
				else
					break;
			}
			while (s.size() && (j == 0 || j == 2))
			{
				if (s[0] == ' ' || s[0] == '\r' || s[0] == '\n' || s[0] == '\t')
					s.erase(s.begin());
				else
					break;
			}
			return s;
		}

		inline vector<char>& TrimOnce(vector<char>& s)
		{
			if (s.size())
			{
				if (strncmp(s.data() + s.size() - 2, "\r\n", 2) == 0)
				{
					s.erase(s.end() - 1);
					s.erase(s.end() - 1);
				}
				else
					if (strncmp(s.data() + s.size() - 1, "\n", 1) == 0)
					{
						s.erase(s.end() - 1);
					}
			}
			return s;
		}


		inline void Split(const char* m, const char* del, vector<string>& result)
		{
			if (!m || !del)
				return;
			size_t pos = 0;
			std::string token;
			string delimiter = del;
			string s = m;
			while ((pos = s.find(delimiter)) != std::string::npos)
			{
				token = s.substr(0, pos);
				result.push_back(token);
				s.erase(0, pos + delimiter.length());
			}
			result.push_back(s);
		}


		inline void BinarySplit(const char* m, size_t sz, const char* del, vector<vector<char>>& result)
		{
			if (!m || !del)
				return;
			size_t pos = 0;
			std::string token;
			string delimiter = del;
			string s;
			s.assign(m, sz);
			while ((pos = s.find(delimiter)) != std::string::npos)
			{
				token = s.substr(0, pos);
				vector<char> res;
				res.resize(token.size());
				memcpy(res.data(), token.data(), token.size());
				result.push_back(res);
				s.erase(0, pos + delimiter.length());
			}

			vector<char> res;
			res.resize(s.size());
			memcpy(res.data(), s.data(), s.size());
			result.push_back(res);
		}

		class HDRSTRING
		{
			vector<string> strs;

		public:


			vector<string>& getstrings() { return strs; }

			string Sub(const char* ga) const
			{
				if (!ga)
					return "";
				for (auto& a : strs)
				{
					const char* f1 = strchr(a.c_str(), '=');
					if (!f1)
					{
						if (_stricmp(a.c_str(), ga) == 0)
							return a;
						continue;
					}
					vector<char> leftpart(f1 - a.c_str() + 10);
					strncpy_s(leftpart.data(), leftpart.size(), a.c_str(), f1 - a.c_str());
					if (_strnicmp(leftpart.data(), ga, strlen(ga)) == 0)
					{
						string r = f1 + 1;
						if (r.length() && r[0] == '\"')
							r.erase(r.begin());
						if (r.length() && r[r.length() - 1] == '\"')
							r.erase(r.end() - 1);
						return r;
					}
				}
				return "";
			}

			string rawright;
			MIMEERR Parse(const char* h)
			{
				if (!h)
					return MIMEERR::INVALID;
				rawright = h;
				strs.clear();
				Split(h, ';', strs);


				for (auto& a : strs)
				{
					Trim(a);
				}
				for (signed long long i = strs.size() - 1; i >= 0; i--)
				{
					if (strs[(size_t)i].length() == 0)
						strs.erase(strs.begin() + (size_t)i);
				}

				return MIMEERR::OK;
			}

			string Serialize() const
			{
				string r;
				for (auto& s : strs)
				{
					if (r.length())
						r += "; ";
					r += s;
				}
				return r;
			}


		};

		class HEADER
		{
			string left;
			HDRSTRING right;
			bool http = false;

		public:

			bool IsHTTP() const { return http; }
			const string& LeftC() const { return left; }
			string Left() const { return left; }
			string Right() const { return right.Serialize(); }
			string Right(const char* sub) const { return right.Sub(sub); }
			HDRSTRING& rights() { return right; }

			vector<string> httpsplit()
			{
				vector<string> hd;
				Split(left.c_str(), ' ', hd);
				return hd;
			}


			void operator =(const char* l)
			{
				right.Parse(l);
			}

			MIMEERR Parse(const char* f, bool CanHTTP = false)
			{
				if (!f)
					return MIMEERR::INVALID;
				const char* a = strchr(f, ':');
				if (!a && !CanHTTP)
					return MIMEERR::INVALID;

				const char* a2 = strchr(f, ' ');
				if ((a2 < a) && CanHTTP)
					a = 0;

				if (!a && CanHTTP)
				{
					left = f;
					http = true;
					return MIMEERR::OK;
				}

				vector<char> d;
				d.resize(a - f + 10);
				strncpy_s(d.data(), d.size(), f, a - f);

				left = d.data();
				a++;
				while (*a == ' ')
					a++;
				right.Parse(a);

				return MIMEERR::OK;
			}

			string Serialize() const
			{
				if (http)
					return left;
				string r;
				r += left;
				r += ": ";
				r += right.Serialize();
				return r;
			}


		};


#define SKIP '\202'
#define NOSKIP 'A'

		const char hexmap[] = {
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			0 ,    1 ,    2 ,    3 ,    4 ,    5 ,    6 ,    7 ,
			8 ,    9 ,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,    10,    11,    12,    13,    14,    15,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,
			SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP,   SKIP
		};

		class CONTENT
		{
			std::vector<HEADER> headers;
			vector<char> data;

		private:


			char* QPDecode(char* input)
			{
				char* s = input;
				char* finalresult = (char*)calloc(strlen(input) + sizeof(char), sizeof(char));
				char* result = finalresult;
				while (*s != '\0') //loop through the entire string...
				{
					if (*s == '=') //woops, needs to be decoded...
					{
						for (int i = 0; i < 3; i++) //is s more than 3 chars long...
						{
							if (s[i] == '\0')
							{
								//error in the decoding...
								return finalresult;
							}
						}
						char mid[3];
						s++; //move past the "="
						//let's put the hex part into mid...
						bool ok = true;
						for (int i = 0; i < 2; i++)
						{
							if (hexmap[s[i]] == SKIP)
							{
								//we have an error, or a linebreak, in the encoding...
								ok = false;
								if (s[i] == '\r' && s[i + 1] == '\n')
								{
									s += 2;
									//*(result++) = '\r';
									//*(result++) = '\n';
									break;
								}
								else
								{
									//we have an error in the encoding...
									//s--;
								}
							}
							mid[i] = s[i];
						}
						//now we just have to convert the hex string to an char...
						if (ok)
						{
							s += 2;
							int m = hexmap[mid[0]];
							m <<= 4;
							m |= hexmap[mid[1]];
							*(result++) = (char)m;
						}
					}
					else
					{
						if (*s != '\0') *(result++) = *(s++);
					}
				}

				return finalresult;
			}


		public:

			void clear()
			{
				headers.clear();
				data.clear();
			}

			vector<char> GetData() const
			{
				return data;
			}

			vector<HEADER>& GetHeaders()
			{
				return headers;
			}

			void SetData(vector<char>& x)
			{
				data = x;
			}

			void SetData(const char* a, size_t ss = -1)
			{
				if (ss == -1)
					ss = strlen(a);
				else
				{
					vector<char> d(ss);
					memcpy(d.data(), a, ss);
					SetData(d);
					return;
				}

				string j = a;
				Trim(j);
				if (j.empty())
					return;

				data.resize(j.length());
				memcpy(data.data(), j.c_str(), j.length());
			}

			void DecodeData(vector<char>& d)
			{
				auto a2 = hval("Content-Transfer-Encoding");
				if (_stricmp(a2.c_str(), "base64") == 0)
				{
					DWORD dw = 0;
					CryptStringToBinaryA(data.data(), (DWORD)data.size(), CRYPT_STRING_BASE64, 0, &dw, 0, 0);
					d.resize(dw);
					CryptStringToBinaryA(data.data(), (DWORD)data.size(), CRYPT_STRING_BASE64, (BYTE*)d.data(), &dw, 0, 0);
					return;
				}
				if (_stricmp(a2.c_str(), "quoted-printable") == 0)
				{
					vector<char> nd(data.size() + 10);
					strcpy_s(nd.data(), nd.size(), data.data());
					char* ce = QPDecode(nd.data());
					d.resize(strlen(ce) + 1);
					strcpy_s(d.data(), d.size(), ce);
					free(ce);
					return;
				}
				d = data;
			}

			MIME2::HEADER httphdr() const
			{
				for (auto& a : headers)
				{
					if (a.IsHTTP())
						return a;
				}
				MIME2::HEADER me;
				return me;
			}

			/*			string Content()  const
							{
							string a;
							auto d2 = data;
							d2.resize(d2.size() + 1);
							a = d2.data();
							d2.resize(d2.size() - 1);
							return a;
							}
			*/

			string hval(const char* left) const
			{
				for (auto& a : headers)
				{
					if (_strcmpi(a.Left().c_str(), left) == 0)
						return a.Right();
				}
				return "";
			}
			string hval(const char* left, const char* rpart) const
			{
				for (auto& a : headers)
				{
					if (_strcmpi(a.Left().c_str(), left) == 0)
						return a.Right(rpart);
				}
				return "";
			}

			HEADER& AddHTTPHeader(const char* l)
			{
				HEADER h;
				h.Parse(l, true);
				headers.insert(headers.begin(), h);
				return headers[0];
			}

			HEADER& operator [](const char* l)
			{
				for (auto& h : headers)
				{
					if (_stricmp(h.Left().c_str(), l) == 0)
						return h;
				}
				HEADER h;
				string e = l;
				e += ": ";
				h.Parse(e.c_str());
				headers.push_back(h);
				return headers[headers.size() - 1];
			}

			MIMEERR Parse(const char* f, bool CanHTTP = false, size_t ss = -1)
			{
				if (!f)
					return MIMEERR::INVALID;



				// Until \r\n\r\n
				const char* a2 = strstr(f, "\r\n\r\n");
				int jd = 4;
				const char* a21 = strstr(f, "\n\n");
				if (!a2 && !a21)
				{
					// No headers....
					SetData(f);
					return MIMEERR::OK;
				}
				if (a21 && !a2)
				{
					a2 = a21;
					jd = 2;
				}
				else
					if (!a21 && a2)
					{
						jd = 4;
					}
					else
						if (a21 < a2)
						{
							a2 = a21;
							jd = 2;
						}

				vector<char> hdrs;
				hdrs.resize(a2 - f + 10);
				strncpy_s(hdrs.data(), hdrs.size(), f, a2 - f);

				// Parse them
				vector<string> hd;
				Split(hdrs.data(), '\n', hd);
				for (auto& a : hd)
				{
					HEADER h;
					if ((a[0] == ' ' || a[0] == '\t') && headers.size())
					{
						// Join with previous
						auto& ph = headers[headers.size() - 1];
						ph.rights().getstrings().push_back(Trim(a));
						continue;
					}
					Trim(a);
					auto err = h.Parse(a.c_str(), CanHTTP);
					if (err != MIMEERR::OK)
						return err;
					headers.push_back(h);
				}

				if (ss == -1)
					SetData(a2 + jd);
				else
					SetData(a2 + jd, ss - (a2 - f) - jd);
				return MIMEERR::OK;
			}


			/*			string Serialize() const
							{
							string r = SerializeHeaders();
							if (r.length())
								r += "\r\n";
							r += Content();
							return r;
							}
			*/
			vector<char> SerializeToVector() const
			{
				string r = SerializeHeaders();
				if (r.length())
					r += "\r\n";
				vector<char> x;
				x.resize(r.length());
				memcpy(x.data(), r.c_str(), r.length());
				auto os = x.size();
				x.resize(x.size() + data.size());
				memcpy(x.data() + os, data.data(), data.size());
				return x;
			}

			string SerializeHeaders() const
			{
				string r;
				for (auto& h : headers)
				{
					r += h.Serialize();
					r += "\r\n";
				}
				return r;
			}





		};


		class CONTENTBUILDER
		{
			vector<vector<char>> parts;
			string Boundary;

		public:

			CONTENTBUILDER()
			{
				UUID u = { 0 };
				CoCreateGuid(&u);
				//* test :)
				TCHAR str[1000];
				StringFromGUID2(u, str, 1000);
				char star[1000];
				WideCharToMultiByte(CP_UTF8, 0, str, -1, star, 1000, 0, 0);
				Boundary = star;
			}

			void clear()
			{
				parts.clear();
			}

			void Add(char* Data)
			{
				vector<char> x(strlen(Data));
				memcpy(x.data(), Data, strlen(Data));
				parts.push_back(x);
			}

			void Add(CONTENT& c)
			{
				auto h1 = c.SerializeToVector();
				parts.push_back(h1);
			}

			vector<vector<char>>& GetParts() { return parts; }

			void Build(CONTENT& c, const char* Sign = 0)
			{
				c.clear();
				c["MIME-Version"] = "1.0";
				string a = "multipart/mixed";
				if (Sign)
					a = Sign;
				a += "; boundary=\"";
				a += Boundary;
				a += "\"";
				c["Content-Type"] = a.c_str();

				vector<char> d;
				for (auto& aa : parts)
				{
					string j = "--";
					j += Boundary;
					j += "\r\n";

					vector<char> jj(j.length() + aa.size() + 2);
					memcpy(jj.data(), j.c_str(), j.length());
					memcpy(jj.data() + j.length(), aa.data(), aa.size());
					memcpy(jj.data() + j.length() + aa.size(), "\r\n", 2);
					auto es = d.size();
					d.resize(es + jj.size());
					memcpy(d.data() + es, jj.data(), jj.size());
				}

				auto es = d.size();
				d.resize(es + 2 + Boundary.size() + 4);
				memcpy(d.data() + es, "--", 2);
				memcpy(d.data() + es + 2, Boundary.c_str(), Boundary.size());
				memcpy(d.data() + es + 2 + Boundary.size(), "--\r\n", 4);

				c.SetData(d);
			}
		};

		inline void BuildZ(CONTENT& c1, CONTENT& c2, CONTENT& co, const char* s)
		{
			CONTENTBUILDER cb2;
			cb2.Add(c1);
			cb2.Add(c2);
			cb2.Build(co, s);

		}
		inline MIMEERR ParseMultipleContent2(const char* d, size_t sz, const char* del, vector<CONTENT>& Result)
		{
			if (!d || !del)
				return MIMEERR::INVALID;

			string dx = "--";
			dx += del;
			vector<vector<char>> r;

			BinarySplit(d, sz, dx.c_str(), r);

			if (r.size() < 2)
				return MIMEERR::INVALID;

			string delj = "--";
			delj += del;
			// First, check if [0] starts with it
			if (r[0].size() == 0 || strncmp(r[0].data(), delj.c_str(), delj.length()) != 0)
				r.erase(r.begin());

			// Check last if it starts with --
			if (strncmp(r[r.size() - 1].data(), "--", 2) == 0)
				r.erase(r.end() - 1);
			else
				return MIMEERR::INVALID;


			for (auto& a : r)
			{
				CONTENT c;
				Trim(a, 2);
				TrimOnce(a);
				auto ra = a;
				ra.resize(ra.size() + 1);
				auto err = c.Parse(ra.data(), 0, ra.size() - 1);
				if (err != MIMEERR::OK)
					return err;

				Result.push_back(c);
			}


			return MIMEERR::OK;
		}



	}


namespace AXLIBRARY
{
#pragma warning(disable:4100)

	// messages
#define AX_QUERYINTERFACE (WM_USER + 1)
#define AX_INPLACE (WM_USER + 2)
#define AX_GETAXINTERFACE (WM_USER + 3)
#define AX_CONNECTOBJECT (WM_USER + 4)
#define AX_DISCONNECTOBJECT (WM_USER + 5)
#define AX_SETDATAADVISE (WM_USER + 6)
#define AX_DOVERB (WM_USER + 7)
#define AX_SETCOMMANDCALLBACK  (WM_USER + 8)
#define AX_SETDISPATCHNOTIFICATION (WM_USER + 9)
// #define AX_SETSERVICEPROVIDER (WM_USER + 10)
#define AX_SETDISPATCHNOTIFICATIONFUNC (WM_USER + 11)
#define AX_RECREATE (WM_USER + 12)
#define AX_RECREATEFROMDATA (WM_USER + 150)
#define AX_SETPARENT (WM_USER + 15)


// notifications
#define AXN_SIZE 1


// Registration function
	ATOM AXRegister();
	int AXConnectObject(IUnknown* Container, REFIID riid, IUnknown* Advisor, IConnectionPointContainer** picpc, IConnectionPoint** picp);
	void AXDisconnectObject(IConnectionPointContainer* icpc, IConnectionPoint* icp, unsigned int Cookie);

	struct AX_CONNECTSTRUCT
	{
		IOleClientSite* Advisor;
		IConnectionPointContainer* icpc;
		IConnectionPoint* icp;
		DWORD id;
		CLSID SpecialIIDForceOK;
	};


#pragma warning(disable:4584)
	// Class AXClientSide
	class AXClientSite :
		public IOleClientSite,
		public IUnknown,
		public IServiceProvider,
		public IDispatch,
		public IAdviseSink,
		public IOleInPlaceSite,
		public IOleInPlaceFrame
	{
	protected:

		int refNum;

	public:


		HWND Window;
		HWND Parent;
		HMENU Menu;
		bool InPlace;
		int ExternalPlace;
		bool CalledCanInPlace;
		CLSID SpecialIIDForceOK;

		class AX* ax;

		// MyClientSite Methods
		AXClientSite();
		virtual ~AXClientSite();
		STDMETHODIMP_(void) OnDataChange2(FORMATETC*);

		// IUnknown methods
		STDMETHODIMP QueryInterface(REFIID iid, void** ppvObject);
		STDMETHODIMP_(ULONG) AddRef();
		STDMETHODIMP_(ULONG) Release();

		// IServiceProvider methods
		STDMETHODIMP QueryService(REFGUID guid, REFIID iid, void** ppvObject);

		// IOleClientSite methods
		STDMETHODIMP SaveObject();
		STDMETHODIMP GetMoniker(DWORD dwA, DWORD dwW, IMoniker** pm);
		STDMETHODIMP GetContainer(IOleContainer** pc);
		STDMETHODIMP ShowObject();
		STDMETHODIMP OnShowWindow(BOOL f);
		STDMETHODIMP RequestNewObjectLayout();

		// IAdviseSink methods
		STDMETHODIMP_(void) OnDataChange(FORMATETC* pFormatEtc, STGMEDIUM* pStgmed);

		STDMETHODIMP_(void) OnViewChange(DWORD dwAspect, LONG lIndex);
		STDMETHODIMP_(void) OnRename(IMoniker* pmk);
		STDMETHODIMP_(void) OnSave();
		STDMETHODIMP_(void) OnClose();

		// IOleInPlaceSite methods
		STDMETHODIMP GetWindow(HWND* p);
		STDMETHODIMP ContextSensitiveHelp(BOOL);
		STDMETHODIMP CanInPlaceActivate();
		STDMETHODIMP OnInPlaceActivate();
		STDMETHODIMP OnUIActivate();
		STDMETHODIMP GetWindowContext(IOleInPlaceFrame** ppFrame, IOleInPlaceUIWindow** ppDoc, LPRECT r1, LPRECT r2, LPOLEINPLACEFRAMEINFO o);
		STDMETHODIMP Scroll(SIZE s);
		STDMETHODIMP OnUIDeactivate(int);
		STDMETHODIMP OnInPlaceDeactivate();
		STDMETHODIMP DiscardUndoState();
		STDMETHODIMP DeactivateAndUndo();
		STDMETHODIMP OnPosRectChange(LPCRECT);

		// IOleInPlaceFrame methods
		STDMETHODIMP GetBorder(LPRECT l);
		STDMETHODIMP RequestBorderSpace(LPCBORDERWIDTHS);
		STDMETHODIMP SetBorderSpace(LPCBORDERWIDTHS w);
		STDMETHODIMP SetActiveObject(IOleInPlaceActiveObject* pV, LPCOLESTR s);
		STDMETHODIMP InsertMenus(HMENU h, LPOLEMENUGROUPWIDTHS x);
		STDMETHODIMP SetMenu(HMENU h, HOLEMENU hO, HWND hw);
		STDMETHODIMP RemoveMenus(HMENU h);
		STDMETHODIMP SetStatusText(LPCOLESTR t);
		STDMETHODIMP EnableModeless(BOOL f);
		STDMETHODIMP TranslateAccelerator(LPMSG, WORD);


		// IDispatch Methods
		HRESULT _stdcall GetTypeInfoCount(unsigned int* pctinfo);
		HRESULT _stdcall GetTypeInfo(unsigned int iTInfo, LCID lcid, ITypeInfo FAR* FAR* ppTInfo);
		HRESULT _stdcall GetIDsOfNames(REFIID riid, OLECHAR FAR* FAR*, unsigned int cNames, LCID lcid, DISPID FAR*);
		HRESULT _stdcall Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS FAR* pDispParams, VARIANT FAR* pVarResult, EXCEPINFO FAR* pExcepInfo, unsigned int FAR* puArgErr);

		// IOleControlSite Methods
	};




	// Class AX
	class AX
	{
	public:

		friend class AXClientSite;
		AX(char* clsid);
		CLSID GetCLSID();
		~AX();


		void Init(char* clsid);
		void Clean();

		AXClientSite Site;
		IID* iid;

		//_COM_SMARTPTR_TYPEDEF(IOleObject,__uuidof(IOleObject));
		IOleObjectPtr OleObject;
		//		_com_ptr_t<IOleObject> OleObject;
				//_com_ptr_t<IStorage> Storage;
		IStoragePtr Storage;

		//_com_ptr_t<IViewObject> View;
		IViewObjectPtr View;

		// _com_ptr_t<IDataObject> Data;
		IDataObjectPtr Data;

		//_com_ptr_t<IOleInPlaceActiveObject> Pao;
		IOleInPlaceActiveObjectPtr Pao;

		//_com_ptr_t<IServiceProvider> CustomServiceProvider;
		IServiceProviderPtr CustomServiceProvider;

		AX_CONNECTSTRUCT* tcs;
		bool AddMenu;
		DWORD AdviseToken;
		DWORD DAdviseToken[100];
		HWND CommandCallbackWindow;
		HWND DispatchNotificationWindow;
		void(__stdcall* DispatchNotificationFunction)(class AXDISPATCHNOTIFICATION*);
		UINT DispatchNotificationMessage;
		void SetProcessCmd(HRESULT(*y)(char*, LPARAM)) { pcmd = y; }

		HWND hPar = 0;
		UINT hClosingMessage = 0;


	private:

		CLSID clsid;
		HRESULT(*pcmd)(char*, LPARAM);


	};


	class AXDISPATCHNOTIFICATION
	{
	public:

		class AX* ax = 0;
		DISPID dispIdMember = 0;
		CLSID riid = {};
		LCID lcid = {};
		WORD wFlags = {};
		DISPPARAMS FAR* pDispParams = 0;
		VARIANT FAR* pVarResult = 0;
		EXCEPINFO FAR* pExcepInfo = 0;
		unsigned int FAR* puArgErr = 0;

		/*
			AXDISPATCHNOTIFICATION()
				{
				ax = 0;
				dispIdMember = 0;
				memset(&riid,0,sizeof(riid));
				wFlags = 0;
				lcid = 0;
				pDispParams = 0;
				pVarResult = 0;
				pExcepInfo = 0;
				puArgErr = 0;
				}*/
	};


	// AXClientSite class
	// ------- Implement member functions
	inline AXClientSite::AXClientSite()
	{
		refNum = 0;
		CalledCanInPlace = 0;
		InPlace = 0;
	}

	inline AXClientSite :: ~AXClientSite()
	{
	}


	// IUnknown methods
	inline STDMETHODIMP AXClientSite::QueryInterface(REFIID iid, void** ppvObject)
	{
		*ppvObject = 0;
		if (iid == IID_IOleClientSite)
			*ppvObject = (IOleClientSite*)this;
		if (iid == IID_IUnknown)
			*ppvObject = this;
		if (iid == IID_IAdviseSink)
			*ppvObject = (IAdviseSink*)this;
		if (iid == IID_IDispatch)
			*ppvObject = (IDispatch*)this;
		if (iid == SpecialIIDForceOK)
			*ppvObject = (IDispatch*)this;
		if (iid == IID_IServiceProvider)
			*ppvObject = (IServiceProvider*)this;
		if (ExternalPlace == false)
		{
			if (iid == IID_IOleInPlaceSite)
				*ppvObject = (IOleInPlaceSite*)this;
			if (iid == IID_IOleInPlaceFrame)
				*ppvObject = (IOleInPlaceFrame*)this;
			if (iid == IID_IOleInPlaceUIWindow)
				*ppvObject = (IOleInPlaceUIWindow*)this;
		}

		//* Log Call
		if (*ppvObject)
		{
			this->AddRef();
			return S_OK;
		}
		return E_NOINTERFACE;
	}

	inline STDMETHODIMP_(ULONG)  AXClientSite::AddRef()
	{
		refNum++;
		return refNum;
	}

	inline STDMETHODIMP_(ULONG)  AXClientSite::Release()
	{
		refNum--;
		return refNum;
	}

	// IServiceManager 	
	inline STDMETHODIMP AXClientSite::QueryService(REFGUID guid, REFIID iid, void** ppvObject)
	{
		if (!ax->CustomServiceProvider.GetInterfacePtr())
			return E_NOINTERFACE;
		return ax->CustomServiceProvider->QueryService(guid, iid, ppvObject);
	}


	// IOleClientSite methods
	inline STDMETHODIMP AXClientSite::SaveObject()
	{
		return S_OK;
	}

	inline STDMETHODIMP AXClientSite::GetMoniker(DWORD dwA, DWORD dwW, IMoniker** pm)
	{
		*pm = 0;
		return E_NOTIMPL;
	}

	inline STDMETHODIMP AXClientSite::GetContainer(IOleContainer** pc)
	{
		*pc = 0;
		return E_FAIL;
	}

	inline STDMETHODIMP AXClientSite::ShowObject()
	{
		return S_OK;
	}

	inline STDMETHODIMP AXClientSite::OnShowWindow(BOOL f)
	{
		InvalidateRect(Window, 0, TRUE);
		InvalidateRect(Parent, 0, TRUE);
		return S_OK;
	}

	inline STDMETHODIMP AXClientSite::RequestNewObjectLayout()
	{
		return S_OK;
	}

	inline STDMETHODIMP_(void) AXClientSite::OnViewChange(DWORD dwAspect, LONG lIndex)
	{
	}

	inline STDMETHODIMP_(void) AXClientSite::OnRename(IMoniker* pmk)
	{
	}

	inline STDMETHODIMP_(void) AXClientSite::OnSave()
	{
	}

	inline STDMETHODIMP_(void) AXClientSite::OnClose()
	{
	}

	// IOleInPlaceSite methods
	inline STDMETHODIMP AXClientSite::GetWindow(HWND* p)
	{
		*p = Window;
		return S_OK;
	}

	inline STDMETHODIMP AXClientSite::ContextSensitiveHelp(BOOL)
	{
		return E_NOTIMPL;
	}


	inline STDMETHODIMP AXClientSite::CanInPlaceActivate()
	{
		if (InPlace)
		{
			CalledCanInPlace = true;
			return S_OK;
		}
		return S_FALSE;
	}

	inline STDMETHODIMP AXClientSite::OnInPlaceActivate()
	{
		return S_OK;
	}

	inline STDMETHODIMP AXClientSite::OnUIActivate()
	{
		return S_OK;
	}

	inline STDMETHODIMP AXClientSite::GetWindowContext(IOleInPlaceFrame** ppFrame, IOleInPlaceUIWindow** ppDoc, LPRECT r1, LPRECT r2, LPOLEINPLACEFRAMEINFO o)
	{
		*ppFrame = (IOleInPlaceFrame*)this;
		AddRef();

		*ppDoc = NULL;
		GetClientRect(Window, r1);
		GetClientRect(Window, r2);
		o->cb = sizeof(OLEINPLACEFRAMEINFO);
		o->fMDIApp = false;
		o->hwndFrame = Parent;
		o->haccel = 0;
		o->cAccelEntries = 0;

		return S_OK;
	}

	inline STDMETHODIMP AXClientSite::Scroll(SIZE s)
	{
		return E_NOTIMPL;
	}

	inline STDMETHODIMP AXClientSite::OnUIDeactivate(int)
	{
		return S_OK;
	}

	inline STDMETHODIMP AXClientSite::OnInPlaceDeactivate()
	{
		return S_OK;
	}

	inline STDMETHODIMP AXClientSite::DiscardUndoState()
	{
		return S_OK;
	}

	inline STDMETHODIMP AXClientSite::DeactivateAndUndo()
	{
		return S_OK;
	}

	inline STDMETHODIMP AXClientSite::OnPosRectChange(LPCRECT)
	{
		return S_OK;
	}


	// IOleInPlaceFrame methods
	inline STDMETHODIMP AXClientSite::GetBorder(LPRECT l)
	{
		GetClientRect(Window, l);
		return S_OK;
	}

	inline STDMETHODIMP AXClientSite::RequestBorderSpace(LPCBORDERWIDTHS b)
	{
		//return S_OK;
		return E_NOTIMPL;
	}

	inline STDMETHODIMP AXClientSite::SetBorderSpace(LPCBORDERWIDTHS b)
	{
		return S_OK;
	}

	inline STDMETHODIMP AXClientSite::SetActiveObject(IOleInPlaceActiveObject* pV, LPCOLESTR s)
	{
		ax->Pao = pV;
		return S_OK;
	}


	inline STDMETHODIMP AXClientSite::SetStatusText(LPCOLESTR t)
	{
		return E_NOTIMPL;
	}

	inline STDMETHODIMP AXClientSite::EnableModeless(BOOL f)
	{
		return E_NOTIMPL;
	}

	inline STDMETHODIMP AXClientSite::TranslateAccelerator(LPMSG, WORD)
	{
		return E_NOTIMPL;
	}

	// IDispatch Methods
	inline HRESULT _stdcall AXClientSite::GetTypeInfoCount(
		unsigned int* pctinfo) {
		return E_NOTIMPL;
	}

	inline HRESULT _stdcall AXClientSite::GetTypeInfo(
		unsigned int iTInfo,
		LCID lcid,
		ITypeInfo FAR* FAR* ppTInfo) {
		return E_NOTIMPL;
	}

	inline HRESULT _stdcall AXClientSite::GetIDsOfNames(
		REFIID riid,
		OLECHAR FAR* FAR*,
		unsigned int cNames,
		LCID lcid,
		DISPID FAR*) {
		return E_NOTIMPL;
	}


	// Other Methods
	inline void AX::Init(char* cls)
	{
		pcmd = 0;
		wchar_t x[1000] = { 0 };
		MultiByteToWideChar(CP_ACP, 0, cls, -1, x, 1000);
		CLSIDFromString(x, &clsid);
		iid = (IID*)&IID_IOleObject;
		OleObject = 0;
		Storage = 0;
		View = 0;
		Data = 0;
		Pao = 0;
		AdviseToken = 0;
		CustomServiceProvider = 0;
		memset(DAdviseToken, 0, sizeof(DAdviseToken));
		Site.ax = this;
		DispatchNotificationFunction = 0;
		DispatchNotificationWindow = 0;
		DispatchNotificationMessage = 0;
	}

	inline AX::AX(char* cls)
	{
		Init(cls);
	}



	inline void AX::Clean()
	{
		if (Site.InPlace == true)
		{
			Site.InPlace = false;
			IOleInPlaceObjectPtr iib = 0;
			if (OleObject.GetInterfacePtr())
				OleObject->QueryInterface(IID_IOleInPlaceObject, (void**)&iib);
			if (iib.GetInterfacePtr())
			{
				iib->UIDeactivate();
				iib->InPlaceDeactivate();
			}
		}

		if (AdviseToken && OleObject.GetInterfacePtr())
		{
			OleObject->Unadvise(AdviseToken);
			AdviseToken = 0;
		}
		if (Data.GetInterfacePtr())
		{
			for (int i = 0; i < 100; i++)
				if (DAdviseToken[i])
					Data->DUnadvise(DAdviseToken[i]);
			memset(DAdviseToken, 0, sizeof(DAdviseToken));
		}


		Pao = 0;
		Data = 0;
		View = 0;
		Storage = 0;
		OleObject = 0;
	}

	inline AX :: ~AX()
	{
		Clean();
	}

	inline CLSID AX::GetCLSID()
	{
		return clsid;
	}


	inline HRESULT _stdcall AXClientSite::InsertMenus(HMENU h, LPOLEMENUGROUPWIDTHS x)
	{
		/*      AX * t = (AX*)ax;
		if (t->AddMenu)
		{
		x->width[0] = 0;
		x->width[2] = 0;
		x->width[4] = 0;
		//InsertMenu(h,0,MF_BYPOSITION | MF_POPUP,(int)Menu,"test");
		return S_OK;
		}
		*/
		return E_NOTIMPL;
	}

	inline HRESULT _stdcall AXClientSite::SetMenu(HMENU h, HOLEMENU hO, HWND hw)
	{
		//	AX * t = (AX*)ax;
		/*      if (t->AddMenu)
		{
		if (!h && !hO)
		{
		//::SetMenu(Window,Menu);
		//DrawMenuBar(Window);
		::SetMenu(Parent,Menu);
		DrawMenuBar(Parent);
		return S_OK;
		}

		//::SetMenu(Window,h);
		//DrawMenuBar(Window);

		//HMENU hm = GetMenu(Parent);
		//AppendMenu(hm,MF_POPUP | MF_MENUBREAK,(int)h,0);
		//::SetMenu(Parent,hm);
		::SetMenu(Parent,h);
		DrawMenuBar(Parent);

		//hOleWindow = hw;
		//OleSetMenuDescriptor(hO,Window,hw,0,0);
		OleSetMenuDescriptor(hO,Parent,hw,0,0);

		return S_OK;
		}
		*/
		return E_NOTIMPL;
	}

	inline HRESULT _stdcall AXClientSite::RemoveMenus(HMENU h)
	{
#ifdef WINCE
		return E_NOTIMPL;
#else
		AX* t = (AX*)ax;
		if (t->AddMenu)
		{
			if (!h)
				return S_OK;

			int c = GetMenuItemCount(h);
			for (int i = c; i >= 0; i--)
			{
				HMENU hh = GetSubMenu(h, i);
				if (hh == Menu)
					RemoveMenu(h, i, MF_BYPOSITION);
			}
			if (h == Menu)
				DestroyMenu(h);

			//DrawMenuBar(Window);
			DrawMenuBar(Parent);
			return S_OK;
		}
		return E_NOTIMPL;
#endif
	}

	//extern HRESULT ProcessCmd(char*);
	inline HRESULT _stdcall AXClientSite::Invoke(
		DISPID dispIdMember,
		REFIID riid,
		LCID lcid,
		WORD wFlags,
		DISPPARAMS FAR* pDispParams,
		VARIANT FAR* pVarResult,
		EXCEPINFO FAR* pExcepInfo,
		unsigned int FAR* puArgErr)
	{
		AXDISPATCHNOTIFICATION axd = { ax,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr };
		if (ax->DispatchNotificationWindow)
		{
			SendMessage(ax->DispatchNotificationWindow, ax->DispatchNotificationMessage, 0, (LPARAM)&axd);
		}
		if (ax->DispatchNotificationFunction)
		{
			ax->DispatchNotificationFunction(&axd);
			return S_OK;
		}

		if (ax->pcmd == 0)
			return S_OK;

		// Check for DWebBrowserEvent2 :: BeforeNavigate2
		// dispid = 0xfa
		char zv[1000] = { 0 };

		if (dispIdMember == 0xfa && pDispParams->cArgs == 7)
		{
			for (unsigned int i = 0; i < pDispParams->cArgs; i++)
			{
				if (pDispParams->rgvarg[i].vt == (VT_VARIANT | VT_BYREF))
				{
					VARIANT* xv = pDispParams->rgvarg[i].pvarVal;
					if (xv->vt == VT_BSTR)
						WideCharToMultiByte(0, 0, xv->bstrVal, -1, zv, 1000, 0, 0);
					if (strncmp(zv, "app:", 4) == 0)
					{
						for (unsigned int x = 0; x < pDispParams->cArgs; x++)
						{
							if (pDispParams->rgvarg[x].vt == (VT_BOOL | VT_BYREF))
							{
								VARIANT_BOOL* y = pDispParams->rgvarg[x].pboolVal;
								*y = VARIANT_TRUE;
								break;
							}
						}
						return ax->pcmd(zv, (LPARAM)ax);
					}
				}
				if (pDispParams->rgvarg[i].vt == VT_BSTR)
				{
					VARIANT* xv = &pDispParams->rgvarg[i];
					if (xv->vt == VT_BSTR)
						WideCharToMultiByte(0, 0, xv->bstrVal, -1, zv, 1000, 0, 0);
					if (strncmp(zv, "app:", 4) == 0)
					{
						for (unsigned int x = 0; x < pDispParams->cArgs; x++)
						{
							if (pDispParams->rgvarg[x].vt == (VT_BOOL | VT_BYREF))
							{
								VARIANT_BOOL* y = pDispParams->rgvarg[x].pboolVal;
								*y = VARIANT_TRUE;
								break;
							}
						}
						return ax->pcmd(zv, (LPARAM)ax);
					}
				}
			}
			return S_OK;
		}

		return S_OK;
	}


	inline void _stdcall AXClientSite::OnDataChange(FORMATETC* pFormatEtc, STGMEDIUM* pStgmed)
	{
		// Notify our app that a change is being requested
		return;
	}



	// Window Procedure for AX control
	inline LRESULT CALLBACK AXWndProc(HWND hh, UINT mm, WPARAM ww, LPARAM ll)
	{
		if (mm == WM_CLOSE)
		{
			AX* ax = (AX*)GetWindowLongPtr(hh, GWLP_USERDATA);
			if (!ax)
				return 0;
			if (!ax->hPar)
				return 0;
			auto st = GetWindowLong(hh, GWL_STYLE);
			if (st & WS_CHILD)
				return DefWindowProc(hh, mm, ww, ll);
			return SendMessage(ax->hPar, ax->hClosingMessage, (WPARAM)hh, 0);
		}
		if (mm == WM_CREATE || mm == AX_RECREATE)
		{
			AX* ax;
			char tit[1000] = { 0 };
			HRESULT hr;
			wchar_t wtit[1000] = { 0 };
			GetWindowTextW(hh, wtit, 1000);
			WideCharToMultiByte(CP_ACP, 0, wtit, -1, tit, 1000, 0, 0);
			if (mm == WM_CREATE)
			{
				if (tit[0] == '}')
					return 0; // No Creation at the momemt
			}
			if (mm == AX_RECREATE) // ll is the IUnknown
			{
				tit[0] = '{';
			}


			ax = new AX(tit);
			SetWindowLongPtr(hh, GWLP_USERDATA, (LONG_PTR)ax);
			ax->Site.Window = hh;
			ax->Site.ax = ax;
			ax->Site.Parent = GetParent(hh);

			hr = StgCreateDocfile(0, STGM_READWRITE | STGM_SHARE_EXCLUSIVE | STGM_DIRECT | STGM_CREATE, 0, &ax->Storage);
			ax->Site.Window = hh;

			REFIID rid = *ax->iid;
			if (mm == WM_CREATE)
			{
				CLSID cid = ax->GetCLSID();
				hr = OleCreate(cid, rid, OLERENDER_DRAW, 0, &ax->Site, ax->Storage.GetInterfacePtr(), (void**)&ax->OleObject);
				if (FAILED(hr))
				{
					hr = OleCreate(cid, rid, OLERENDER_NONE, 0, &ax->Site, ax->Storage.GetInterfacePtr(), (void**)&ax->OleObject);
				}
			}
			else
				if (mm == AX_RECREATE)
				{
					IUnknown* u = (IUnknown*)ll;
					if (u)
						u->QueryInterface(rid, (void**)&ax->OleObject);
				}

			if (!ax->OleObject.GetInterfacePtr())
			{
				delete ax;
				SetWindowLongPtr(hh, GWLP_USERDATA, 0);
				return -1;
			}
			ax->OleObject->SetClientSite(&ax->Site);

			hr = OleSetContainedObject(ax->OleObject.GetInterfacePtr(), TRUE);
			hr = ax->OleObject->Advise(&ax->Site, &ax->AdviseToken);
			hr = ax->OleObject->QueryInterface(IID_IViewObject, (void**)&ax->View);
			hr = ax->OleObject->QueryInterface(IID_IDataObject, (void**)&ax->Data);
			if (ax->View.GetInterfacePtr())
				hr = ax->View->SetAdvise(DVASPECT_CONTENT, 0, &ax->Site);



			return 0;
		}



		if (mm == WM_DESTROY)
		{
			AX* ax = (AX*)GetWindowLongPtr(hh, GWLP_USERDATA);
			if (!ax)
				return 0;
			ax->Clean();
			return true;
		}


		if (mm == WM_COMMAND)
		{
			AX* ax = (AX*)GetWindowLongPtr(hh, GWLP_USERDATA);
			if (!ax)
				return 0;

			return SendMessage(ax->CommandCallbackWindow, mm, ww, ll);
		}

		/*
		if (mm == AX_SETSERVICEPROVIDER)
		{
		AX* ax = (AX*)GetWindowLongPtr(hh,GWLP_USERDATA);
		if (!ax)
		return 0;
		ax->CustomServiceProvider = (IServiceProvider*)ll;
		return 1;
		}
		*/

		if (mm == AX_CONNECTOBJECT)
		{
			// ww = (IID*)  Interface ID to which we request connection
			// ll = (void*) Pointer to a AX_CONNECTSTRUCT to fill in
			AX* ax = (AX*)GetWindowLongPtr(hh, GWLP_USERDATA);
			if (!ax)
				return 0;

			int* wx = (int*)ww;
			AX_CONNECTSTRUCT* tcs = (AX_CONNECTSTRUCT*)ll;

			if (tcs->Advisor == 0)
			{
				tcs->Advisor = (IOleClientSite*)&ax->Site;
				ax->Site.SpecialIIDForceOK = tcs->SpecialIIDForceOK;
			}
			tcs->id = AXConnectObject((IUnknown*)ax->OleObject.GetInterfacePtr(), (REFIID)*wx, tcs->Advisor, &tcs->icpc, &tcs->icp);
			ax->tcs = new AX_CONNECTSTRUCT;
			memcpy(ax->tcs, tcs, sizeof(AX_CONNECTSTRUCT));
			return 0;
		}

		if (mm == AX_DISCONNECTOBJECT)
		{
			// Use of ConnectObject
			// ww = (IID*)  Interface ID to which we request connection
			//	char* p = (char*)ww;
			AX* ax = (AX*)GetWindowLongPtr(hh, GWLP_USERDATA);
			if (!ax)
				return 0;

			AX_CONNECTSTRUCT* tcs = ax->tcs;
			AXDisconnectObject(tcs->icpc, tcs->icp, tcs->id);
			delete ax->tcs;
			ax->tcs = 0;
			return 0;
		}


		if (mm == AX_SETCOMMANDCALLBACK)
		{
			AX* ax = (AX*)GetWindowLongPtr(hh, GWLP_USERDATA);
			if (!ax)
				return 0;

			ax->CommandCallbackWindow = (HWND)ww;
			return 1;
		}

		if (mm == AX_SETDISPATCHNOTIFICATION)
		{
			AX* ax = (AX*)GetWindowLongPtr(hh, GWLP_USERDATA);
			if (!ax)
				return 0;

			// ww = Window
			// ll = Message
			ax->DispatchNotificationWindow = (HWND)ww;
			ax->DispatchNotificationMessage = (UINT)ll;
			return 1;
		}

		if (mm == AX_SETDISPATCHNOTIFICATIONFUNC)
		{
			AX* ax = (AX*)GetWindowLongPtr(hh, GWLP_USERDATA);
			if (!ax)
				return 0;

			ax->DispatchNotificationFunction = (void(__stdcall*)(AXDISPATCHNOTIFICATION*))ww;
			return 1;
		}

		if (mm == AX_SETDATAADVISE)
		{
			AX* ax = (AX*)GetWindowLongPtr(hh, GWLP_USERDATA);
			if (!ax)
				return 0;

			// 1.Enum available FORMATETC structures
			// 2.Set Data Advise specified to index ww
			if (!ax->Data.GetInterfacePtr())
				return 0;

			IEnumFORMATETCPtr ief = 0;
			int i = 0;

			FORMATETC fe;
			ax->Data->EnumFormatEtc((DWORD)ll, &ief);
			if (!ief.GetInterfacePtr())
				return 0;
			for (;;)
			{
				HRESULT hr = ief->Next(1, &fe, 0);
				if (hr != S_OK)
					break;
				if (ww == (WPARAM)i)
					break;
				i++;
			}
			if (ww == -1)
				return i;

			if (ax->Data.GetInterfacePtr())
				ax->Data->DAdvise(&fe, 0, &ax->Site, &ax->DAdviseToken[ww]);

			return true;
		}

		if (mm == AX_GETAXINTERFACE)
		{
			AX* ax = (AX*)GetWindowLongPtr(hh, GWLP_USERDATA);
			return (LONG_PTR)ax;
		}

		if (mm == AX_QUERYINTERFACE)
		{
			char* p = (char*)ww;
			AX* ax = (AX*)GetWindowLongPtr(hh, GWLP_USERDATA);
			if (!ax)
				return 0;
			return ax->OleObject->QueryInterface((REFIID)*p, (void**)ll);
		}

		if (mm == WM_LBUTTONDBLCLK)
		{
			PostMessage(hh, AX_INPLACE, 1, 0);
			return 0;
		}

		if (mm == AX_SETPARENT)
		{
			AX* ax = (AX*)GetWindowLongPtr(hh, GWLP_USERDATA);
			if (!ax)
				return 0;
			ax->hPar = (HWND)ww;
			ax->hClosingMessage = (UINT)ll;
			return 0;
		}


		if (mm == AX_INPLACE)
		{
			AX* ax = (AX*)GetWindowLongPtr(hh, GWLP_USERDATA);
			if (!ax)
				return 0;
			if (!ax->OleObject)
				return 0;
			RECT rect;
			HRESULT hr;
			::GetClientRect(hh, &rect);

			if (ax->Site.InPlace == false && ww == 1) // Activate In Place
			{
				ax->Site.InPlace = true;
				ax->Site.ExternalPlace = (int)ll;
				hr = ax->OleObject->DoVerb(OLEIVERB_INPLACEACTIVATE, 0, &ax->Site, 0, hh, &rect);
				InvalidateRect(hh, 0, true);
				return 1;
			}

			if (ax->Site.InPlace == true && ww == 0) // Deactivate
			{
				ax->Site.InPlace = false;

				IOleInPlaceObjectPtr iib;
				ax->OleObject->QueryInterface(IID_IOleInPlaceObject, (void**)&iib);
				if (iib)
				{
					iib->UIDeactivate();
					iib->InPlaceDeactivate();
					InvalidateRect(hh, 0, true);
					return 1;
				}
			}
			return 0;
		}

		if (mm == WM_SIZE)
		{
			AX* ax = (AX*)GetWindowLongPtr(hh, GWLP_USERDATA);
			if (!ax)
				return 0;
			if (!ax->OleObject)
				return 0;

			// Send Notification to parent
			NMHDR nh = { 0 };
			nh.code = AXN_SIZE;
			nh.hwndFrom = hh;
			nh.idFrom = GetWindowLong(hh, GWL_ID);
			SendMessage(ax->Site.Parent, WM_NOTIFY, 0, (LPARAM)&nh);
			DefWindowProc(hh, mm, ww, ll);
			/*
			if (ax->Site.InPlace == true)
			{
			SendMessage(hh,AX_INPLACE,0,0);
			InvalidateRect(hh,0,true);
			SendMessage(hh,AX_INPLACE,1,0);
			}
			*/

			IOleInPlaceObjectPtr pl;
			ax->OleObject->QueryInterface(IID_IOleInPlaceObject, (void**)&pl);
			if (!pl)
				return 0;
			RECT r;
			GetClientRect(ax->Site.Window, &r);
			pl->SetObjectRects(&r, &r);
			return 0;
		}


		return DefWindowProc(hh, mm, ww, ll);
	}



	inline int AXConnectObject(IUnknown* Container, REFIID riid, IUnknown* Advisor, IConnectionPointContainer** picpc, IConnectionPoint** picp)
	{
		HRESULT hr = 0;
		unsigned long tid = 0;
		IConnectionPointContainer* icpc = 0;
		IConnectionPoint* icp = 0;
		*picpc = 0;
		*picp = 0;

		Container->QueryInterface(IID_IConnectionPointContainer, (void**)&icpc);
		if (icpc)
		{
			*picpc = icpc;
			icpc->FindConnectionPoint(riid, &icp);
			if (icp)
			{
				*picp = icp;
				hr = icp->Advise(Advisor, &tid);
			}
		}
		return tid;
	}

	inline void AXDisconnectObject(IConnectionPointContainer* icpc, IConnectionPoint* icp, unsigned int Cookie)
	{
		//unsigned long hr  = 0;
		icp->Unadvise(Cookie);
		icp->Release();
		icpc->Release();
	}



	// Registration function
	inline ATOM AXRegister()
	{
#ifdef WINCE
		WNDCLASS wC = { 0 };
#else
		WNDCLASSEXW wC = { 0 };
		wC.cbSize = sizeof(wC);
#endif

		wC.style = CS_GLOBALCLASS | CS_DBLCLKS;
		wC.lpfnWndProc = AXWndProc;
		wC.cbWndExtra = sizeof(void*);
		wC.hInstance = GetModuleHandle(0);
		wC.lpszClassName = L"AX_RDP";
#ifdef WINCE
		return RegisterClassW(&wC);
#else
		return RegisterClassExW(&wC);
#endif
	}





}

struct TASK_PARAMS
{
	HICON hIcon = 0;
	HWND hPar = 0;
	std::function<void(HWND, void*,void*)> f = nullptr; 
	std::function<HRESULT(HWND, int)> cmdf = nullptr;
	LPARAM cbparam = 0;
	std::function<HRESULT(HWND, UINT,WPARAM,LPARAM,LPARAM,LPARAM)> cb = nullptr;
	void* param = 0;
	bool Marq = true;
	std::wstring tit = L"RDP";
	std::wstring inst = L"Please wait";
	std::wstring cont = L"Initializing";
	std::wstring ftr;
	std::wstring vtr;
	std::vector<TASKDIALOG_BUTTON> usebuttons;
	bool Checked = 0;
};


inline void LogEvent(const char* tag,DISPID ev)
{
#ifdef _DEBUG
	if (ev == 319 || ev == 320 || ev == 325 || ev == 316)
		return; // no window open/close
	char e[100] = {};
	sprintf_s(e, 100, "%s RDP Event % i\r\n", tag,ev);
	OutputDebugStringA(e);
#endif
}

// Enumerators
inline CComPtr<IDispatch> GetDisp(DISPPARAMS* pDispParams)
{
	VARIANT x = { 0 };
	unsigned int y = 0;
	x.vt = VT_DISPATCH;
	HRESULT hr = DispGetParam(pDispParams, 0, VT_DISPATCH, &x, &y);
	if (FAILED(hr))
		return 0;
	CComPtr<IDispatch> d = x.pdispVal;
	if (!d)
		return 0;
	return d;
}


inline std::vector<char> FetchX(const char* TheLink)
{
	// Create thread that will show download progress
	DWORD Size;
	unsigned long bfs = 1000;
	TCHAR ss[1000];
	DWORD TotalTransferred = 0;
	std::vector<char> aaaa;


	int err = 1;

	HINTERNET hI = 0, hRead = 0;

	hI = InternetOpen(_T("Money"), INTERNET_OPEN_TYPE_DIRECT, 0, 0, 0);
	if (!hI)
		goto finish;
	hRead = InternetOpenUrlA(hI, TheLink, 0, 0, INTERNET_FLAG_NO_CACHE_WRITE, 0);
	if (!hRead)
		goto finish;

	if (!HttpQueryInfo(hRead, HTTP_QUERY_CONTENT_LENGTH, ss, &bfs, 0))
		Size = (DWORD)-1;
	else
		Size = _ttoi(ss);

	for (;;)
	{
		DWORD n;
		char Buff[100010] = { 0 };

		memset(Buff, 0, 100010);
		BOOL  F = InternetReadFile(hRead, Buff, 100000, &n);
		if (F == false)
		{
			err = 2;
			break;
		}
		if (n == 0)
		{
			// End of file !
			err = 0;
			break;
		}
		TotalTransferred += n;

		//Write to File !
		//char xx = Buff[n];
		size_t olds = aaaa.size();
		aaaa.resize(olds + n);
		memcpy(aaaa.data() + olds, Buff, n);

		int NewPos = 0;
		if (Size != -1)
			NewPos = (100 * TotalTransferred) / Size;
	}


finish:
	InternetCloseHandle(hRead);
	InternetCloseHandle(hI);
	return aaaa;
}



template <typename I> void GetEnumerationC(CComPtr<IUnknown> x, std::vector<CComVariant>& y)
{
	CComPtr<I> e = 0;
	if (!x)
		return;
	x->QueryInterface(__uuidof(I), (void**)&e);
	if (!e)
		return;
	e->Reset();
	for (;;)
	{
		CComVariant v;
		if (e->Next(1, &v, 0) != S_OK)
			break;
		y.push_back(v);
	}
}

class BX
{
	std::wstring CurrentDesktopBackground;
	COLORREF BGColor = 0;
	COLORREF cz = 0;
	bool BGChanged = false;

	std::vector<std::tuple<int, int, int>> sps;

public:

	void Anim(bool A)
	{
		if (!A)
			return;

		DWORD eps = 0;
		BOOL br = 0;
		BOOL bv = 0;
		br = SystemParametersInfo(SPI_GETCLIENTAREAANIMATION, 0, &bv, 0);
		if (bv)
		{
			auto a = std::tuple<int, int, int>(SPI_SETCLIENTAREAANIMATION, 0, 1);
			sps.push_back(a);
			bv = 0;
			br = SystemParametersInfo(SPI_SETCLIENTAREAANIMATION, 0, &bv, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
			eps = GetLastError();
		}

		br = SystemParametersInfo(SPI_GETMENUANIMATION, 0, &bv, 0);
		if (bv)
		{
			auto a = std::tuple<int, int, int>(SPI_SETMENUANIMATION, 0, 1);
			sps.push_back(a);
			bv = 0;
			br = SystemParametersInfo(SPI_SETMENUANIMATION, 0, 0, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
			eps = GetLastError();
		}

		br = SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0, &bv, 0);
		if (bv)
		{
			auto a = std::tuple<int, int, int>(SPI_SETDRAGFULLWINDOWS, TRUE, 0);
			sps.push_back(a);
			bv = 0;
			br = SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, 0, 0, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
			eps = GetLastError();
		}


		br = SystemParametersInfo(SPI_GETCOMBOBOXANIMATION, 0, &bv, 0);
		if (bv)
		{
			auto a = std::tuple<int, int, int>(SPI_SETCOMBOBOXANIMATION, 0, 1);
			sps.push_back(a);
			bv = 0;
			br = SystemParametersInfo(SPI_SETCOMBOBOXANIMATION, 0, 0, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
			eps = GetLastError();
		}


		ANIMATIONINFO aa;
		aa.cbSize = sizeof(aa);
		br = SystemParametersInfo(SPI_GETANIMATION, sizeof(aa), &aa, 0);
		if (aa.iMinAnimate != 0)
		{
			auto a = std::tuple<int, int, int>(SPI_SETANIMATION, 0, 1);
			sps.push_back(a);
			aa.cbSize = sizeof(aa);
			aa.iMinAnimate = 0;
			br = SystemParametersInfo(SPI_SETANIMATION, sizeof(aa), &aa, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
			eps = GetLastError();
		}
	}


	void AnimEnd()
	{
		for (auto& a : sps)
		{
			BOOL Vx = std::get<2>(a);
			if (std::get<0>(a) == SPI_SETANIMATION)
			{
				ANIMATIONINFO aa;
				aa.cbSize = sizeof(aa);
				aa.iMinAnimate = 1;
				SystemParametersInfo(SPI_SETANIMATION, sizeof(aa), &aa, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
			}
			else
				SystemParametersInfo(std::get<0>(a), std::get<1>(a), &Vx, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);
		}
		sps.clear();
	}

	void ToggleBackground()
	{
		if (BGChanged)
			RestoreBackground();
		else
			BlackBackground();
	}

	void ToggleAnimations()
	{
		if (sps.empty())
			Anim(true);
		else
			AnimEnd();
	}

	void BlackBackground()
	{
		TCHAR t[1000] = { 0 };
		SystemParametersInfo(SPI_GETDESKWALLPAPER, 1000, t, 0);
		CurrentDesktopBackground = t;
		SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, nullptr, 0);
		BGColor = GetSysColor(COLOR_BACKGROUND);
		int x1 = COLOR_BACKGROUND;
		SetSysColors(1, &x1, &cz);
		BGChanged = true;
	}

	void RestoreBackground()
	{
		if (!BGChanged)
			return;
		if (CurrentDesktopBackground.length())
			SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, (void*)CurrentDesktopBackground.c_str(), 0);
		cz = BGColor;
		int x1 = COLOR_BACKGROUND;
		SetSysColors(1, &x1, &cz);
		BGChanged = false;
	}

	BX(bool A)
	{
		Anim(A);
		if (!A)
			return;
		BlackBackground();
	}
	~BX()
	{
		AnimEnd();
		RestoreBackground();
	}
};



class RDP
{
public:

	HICON hIcon = 0;
	HRESULT WaitTask(std::function<void(HWND,void*,void*)> foo,void* lp)
	{
		TASK_PARAMS tp;
		tp.hIcon = hIcon;
		tp.f = foo;
		tp.param = lp;
		return Task(tp);

	}

	// Generic Task Dialog
	HRESULT Task(TASK_PARAMS task_params)
	{
		TASKDIALOGCONFIG tc = { 0 };
		tc.cbSize = sizeof(tc);
		tc.hwndParent = task_params.hPar;
		tc.pszWindowTitle = task_params.tit.c_str();
		tc.hMainIcon = task_params.hIcon;
		tc.pszMainInstruction = task_params.inst.c_str();
		tc.pszContent = task_params.cont.c_str();
		tc.pszFooter = task_params.ftr.length() ? task_params.ftr.c_str() : L"";
		tc.pszVerificationText = task_params.vtr.length() ? task_params.vtr.c_str() : L"";

		tc.dwCommonButtons = TDCBF_CANCEL_BUTTON;

		TASKDIALOG_BUTTON b2[100] = { 0 };
		if (task_params.usebuttons.size())
		{
			tc.dwCommonButtons = 0;
			tc.cButtons = (UINT)task_params.usebuttons.size();
			tc.pButtons = task_params.usebuttons.data();
		}

		tc.dwFlags = (task_params.Marq ? TDM_SET_PROGRESS_BAR_MARQUEE : 0) | TDF_USE_HICON_MAIN | TDF_CAN_BE_MINIMIZED;
		if (task_params.Checked)
			tc.dwFlags |= TDF_VERIFICATION_FLAG_CHECKED;
		int p1 = 0;
		int p2 = 0;
		BOOL p3 = 0;
		tc.lpCallbackData = (LPARAM)&task_params;


		tc.pfCallback = [](HWND hwnd, UINT uNotification, WPARAM wParam, LPARAM lParam, LONG_PTR dwRefData) -> HRESULT
			{
				HWND hTask = 0;
				TASK_PARAMS* f2 = (TASK_PARAMS*)dwRefData;

				if (f2->cb)
				{
					auto e = f2->cb(hwnd, uNotification,wParam, lParam,dwRefData,f2->cbparam);
					if (e != E_ABORT)
						return e;
				}

				if (uNotification == TDN_CREATED)
				{
					hTask = hwnd;
					SendMessage(hTask, TDM_SET_PROGRESS_BAR_MARQUEE, TRUE, 0);
					if (f2->f)
					{
						std::thread tx(f2->f, hTask, f2, f2->param);
						tx.detach();
						// call call SendMessage(hTask, TDM_CLICK_BUTTON, IDCANCEL, 0);
					}
					return S_OK;
				}
				if (uNotification == TDN_VERIFICATION_CLICKED)
				{
					f2->Checked = (bool)wParam;
					return S_FALSE;
				}

				if (uNotification == TDN_BUTTON_CLICKED)
				{
					if (f2->cmdf)
						return f2->cmdf(hwnd, (int)wParam);
					return S_OK;
				}
				return S_OK;
			};

		HRESULT hr = E_FAIL;
		hr = TaskDialogIndirect(&tc, &p1, &p2, &p3);
		if (p1 == 0)
			return E_FAIL;
		return S_OK;
	}

	SOCKET MainServerSocket = 0;

	class MyStreamBuffer : public IRDPSRAPITransportStreamBuffer
	{
	public:

		std::vector<BYTE> buffer;
		ULONG r = 1;
		size_t actst = 0;
		MyStreamBuffer(long mx)
		{
			actst = 0;
			buffer.resize(mx);
		}
		// IUnknown
		STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject)
		{
			*ppvObject = 0;
			if (riid == __uuidof(IRDPSRAPITransportStreamBuffer))
				*ppvObject = (IRDPSRAPITransportStreamBuffer*)this;
			if (riid == IID_IUnknown)
				*ppvObject = (IUnknown*)this;
			if (*ppvObject)
			{
				AddRef();
				return S_OK;
			}
			return E_NOINTERFACE;
		}
		STDMETHODIMP_(ULONG) AddRef()
		{
			return InterlockedIncrement(&r);
		}
		STDMETHODIMP_(ULONG) Release()
		{
			if (InterlockedDecrement(&r) == 0)
			{
				delete this;
				return 0;
			}
			return r;
		}
		
		
		CComPtr<IUnknown> ctx;
		// IRDPSRAPITransportStreamBuffer
		HRESULT __stdcall get_Context(
			IUnknown** ppContext
		)
		{
			if (!ppContext)
				return E_POINTER;
			*ppContext = ctx;
			if (*ppContext)
				(*ppContext)->AddRef();
			return S_OK;	
		}

		HRESULT __stdcall put_Context(
			IUnknown* pContext
		)
		{
			ctx = pContext;
			return S_OK;
		}

		long _flags = 0;
		HRESULT __stdcall get_Flags(long* pFlags
		)
		{
			if (!pFlags)
				return E_POINTER;
			*pFlags = _flags;
			return S_OK;
		}

		HRESULT __stdcall put_Flags(long Flags
		)
		{
			_flags = Flags;
			return S_OK;
		}

		HRESULT __stdcall get_Storage(
			BYTE** ppbStorage
		)
		{
			if (!ppbStorage)
				return E_POINTER;
			*ppbStorage = buffer.data();
			return S_OK;
		}

		HRESULT __stdcall get_StorageSize(
			long* plSize
		)
		{
			if (!plSize)
				return E_POINTER;
			*plSize = (long)buffer.size();
			return S_OK;
		}

		HRESULT __stdcall put_PayloadSize(
			long lSize
		)
		{
			actst = lSize;
			if (actst > buffer.size())
				buffer.resize(actst);
			return S_OK;
		}

		HRESULT __stdcall get_PayloadSize(
			long* plSize
		)
		{
			if (!plSize)
				return E_POINTER;
			*plSize = (long)actst;
			return S_OK;
		}

		long ofs = 0;
		HRESULT __stdcall put_PayloadOffset(
			long lOffset
		)
		{
			ofs = lOffset;
			return S_OK;
		}

		HRESULT __stdcall get_PayloadOffset(
			long* plOffset
		)
		{
			*plOffset = ofs;
			return S_OK;
		}


	};


	class MyStream : public IRDPSRAPITransportStream
	{
	public:

		ULONG r = 1;
		RDP* rdp = 0;
		SOCKET X = 0;
		MyStream(RDP* r,SOCKET Y = 0)
		{
			X = Y;
			rdp = r;
		}
		CComPtr< IRDPSRAPITransportStreamEvents> events;

		// IUnknown
		STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject)
		{
			*ppvObject = 0;
			if (riid == __uuidof(IRDPSRAPITransportStream))
				*ppvObject = (IRDPSRAPITransportStream*)this;
			if (riid == IID_IUnknown)
				*ppvObject = (IUnknown*)this;
			if (*ppvObject)
			{
				AddRef();
				return S_OK;
			}
			return E_NOINTERFACE;
		}

		STDMETHODIMP_(ULONG) AddRef()
		{
			return InterlockedIncrement(&r);
		}

		STDMETHODIMP_(ULONG) Release()
		{
			if (InterlockedDecrement(&r) == 0)
			{
				delete this;
				return 0;
			}
			return r;
		}

		// IRDPSRAPITransportStream
		HRESULT __stdcall AllocBuffer(
			         long                           maxPayload,
			IRDPSRAPITransportStreamBuffer** ppBuffer
		)
		{
			if (!ppBuffer)
				return E_POINTER;
			auto b = new MyStreamBuffer(maxPayload);
#ifdef _DEBUG
			b->AddRef();
#endif
			*ppBuffer = b;
			return S_OK;
		}

		HRESULT __stdcall Close()
		{
			if (!events)
				return S_FALSE;
			events = 0;
			return S_OK;
		}

		HRESULT __stdcall FreeBuffer(
			IRDPSRAPITransportStreamBuffer* pBuffer
		)
		{
			if (!pBuffer)
				return E_POINTER;
			pBuffer->Release();
			return S_OK;
		}

		HRESULT __stdcall Open(
			IRDPSRAPITransportStreamEvents* pCallbacks
		)
		{
			events = pCallbacks;
			return S_OK;
		}

		HRESULT __stdcall ReadBuffer(
			IRDPSRAPITransportStreamBuffer* pBuffer
		)
		{
			if (!pBuffer)
				return E_POINTER;
			CComPtr<IRDPSRAPITransportStreamBuffer> pp;
			pp = pBuffer;
			std::lock_guard<std::recursive_mutex> l(mut);
			Pending_read.push(pp);
			UpdateReads();
			return S_OK;
		}

		std::recursive_mutex mut;
		std::queue<CComPtr<IRDPSRAPITransportStreamBuffer>> Pending_read;
		std::vector<CComPtr<IRDPSRAPITransportStreamBuffer>> Pending_write;
		std::vector<char> Available_Read;


		void DataReady(std::vector<char>& x)
		{
			if (1)
			{
				std::lock_guard<std::recursive_mutex> l(mut);
				Available_Read.insert(Available_Read.end(), x.begin(), x.end());
			}
			UpdateReads();
		}

		void UpdateWrites()
		{
			std::lock_guard<std::recursive_mutex> l(mut);
			for (auto& wr : Pending_write)
			{
				if (events)
					events->OnWriteCompleted(wr);

			}
			Pending_write.clear();
		}

		void UpdateReads()
		{
			std::lock_guard<std::recursive_mutex> l(mut);
			if (Pending_read.size() == 0) // no buffers here
				return;
			auto pBuffer = Pending_read.front();
			long ps = 0;
			pBuffer->get_StorageSize(&ps);
			if (Available_Read.size() == 0)
				return;

			Pending_read.pop();
			auto how_many = std::min(ps, (long)Available_Read.size());

			long ofs = 0;
			pBuffer->get_PayloadOffset(&ofs);

			pBuffer->put_PayloadOffset(0);
			BYTE* by = 0;
			pBuffer->get_Storage(&by);
			memcpy(by, Available_Read.data(), how_many);
			pBuffer->put_PayloadSize(how_many);
			Available_Read.erase(Available_Read.begin(), Available_Read.begin() + how_many);
			if (events)
				events->OnReadCompleted(pBuffer);
		}

		HRESULT __stdcall WriteBuffer(
			IRDPSRAPITransportStreamBuffer* pBuffer
		)
		{
			// Write immediately
			PACKET p;
			p.w = 1;
			long sz = 0;
			pBuffer->get_PayloadSize(&sz);
			long ofs = 0;
			pBuffer->get_PayloadOffset(&ofs);
			BYTE* by = 0;
			pBuffer->get_Storage(&by);
			p.bytes = sz;
			int vx = ssend(X, (char*)&p, sizeof(PACKET) );
			vx = ssend(X, (char*)by + ofs, (int)p.bytes);
			if (vx != p.bytes)
			{
				return E_FAIL;
			}
			if (events)
				events->OnWriteCompleted(pBuffer);
			return S_OK;
		}
	};

	static int ssend(SOCKET X,const char* b, int sz)
	{
		// same as send, but forces reading ALL sz
		int rs = 0;
		for (;;)
		{
			int tosend = sz - rs;
			//			if (tosend > 10000)
			//				tosend = 10000;
			int rval = send(X, b + rs, tosend,0);
			if (rval == 0 || rval == SOCKET_ERROR)
			{
				auto err = WSAGetLastError();
				err;
				return rs;
			}
			rs += rval;
			if (rs == sz)
				return rs;
		}
	}

	static bool isSocketReady(SOCKET X)
	{

		/// Got here because iSelectReturn > 0 thus data available on at least one descriptor
		// Is our socket in the return list of readable sockets
		bool             res;
		fd_set          sready;
		struct timeval  nowait;

		FD_ZERO(&sready);
		FD_SET((unsigned int)X, &sready);
		//bzero((char *)&nowait,sizeof(nowait));
		memset((char*)&nowait, 0, sizeof(nowait));

		res = select(0, &sready, NULL, NULL, &nowait);
		if (FD_ISSET(X, &sready))
			res = true;
		else
			res = false;


		return res;

	}

	static int rrecv(SOCKET X,char* b, int sz)
	{
		// same as recv, but forces reading ALL sz
		int rs = 0;
		for (;;)
		{
			int rval = recv(X, b + rs, sz - rs,0);
			if (rval == 0 || rval == SOCKET_ERROR)
				return rs;
			rs += rval;
			if (rs == sz)
				return rs;
		}
	}


#pragma pack(push,1)
	struct PACKET
	{
		char w = 0; // 'G' or 'P' -> HTTP else the control
		unsigned long long bytes = 0;
	};
#pragma pack(pop)


	class MyClipboardRedirect : public IRDPSRAPIClipboardUseEvents
	{
		ULONG r = 1;
		RDP* rdp = 0;
	public:


		MyClipboardRedirect(RDP* r)
		{
			rdp = r;
		}
		// IUnknown
		STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject)
		{
			*ppvObject = 0;
			if (riid == __uuidof(IRDPSRAPIClipboardUseEvents))
				*ppvObject = (IRDPSRAPIClipboardUseEvents*)this;
			if (riid == IID_IUnknown)
				*ppvObject = (IUnknown*)this;
			if (*ppvObject)
			{
				AddRef();
				return S_OK;
			}
			return E_NOINTERFACE;
		}
		STDMETHODIMP_(ULONG) AddRef()
		{
			return InterlockedIncrement(&r);
		}
		STDMETHODIMP_(ULONG) Release()
		{
			if (InterlockedDecrement(&r) == 0)
			{
				delete this;
				return 0;
			}
			return r;
		}
		
		HRESULT __stdcall OnPasteFromClipboard(
			 UINT         clipboardFormat,
			 IDispatch* pAttendee,
			 VARIANT_BOOL* pRetVal
		)
		{
			return S_OK;
		}

	};

	void CreateANewServer(SOCKET X,bool MyStreamX,MyStream* ForReverse,int Port,std::wstring* key = 0)
	{
		CoInitialize(0);
		SERVER srv;
		PROPERTIES props;
		if (Port)
			props.PortId = Port;
		MyStream* str = 0;
		MyClipboardRedirect* clip = new MyClipboardRedirect(this);
		if (MyStreamX)
		{
			if (ForReverse)
			{
				str = ForReverse;
				str->AddRef();
			}
			else
				str = new MyStream(this, X);
			props.SetNetworkStream = str;
		}
		props.DefaultAttendeeControlLevel = CTRL_LEVEL::CTRL_LEVEL_VIEW;
		props.EnableClipboardRedirect = true;
		props.SetClipboardRedirectCallback = clip;
		srv.Props(props);
		srv.Open(nullptr);
		srv.ShareAllApplications();

		auto inv = srv.GetInvitation();
		CComBSTR b;
		inv->get_ConnectionString(&b);

		if (b)
		{
			if (key)
				*key = b;
			std::vector<wchar_t> dt(wcslen(b) + 1);
			wcscpy_s(dt.data(), dt.size(), b);
			PACKET p;
			p.w = 0;
			if (MyStreamX)
				p.w = 2;
			p.bytes = dt.size() * 2;
			if (X)
			{
				ssend(X, (char*)&p, sizeof(PACKET));
				ssend(X, (char*)dt.data(), (int)p.bytes);
			}
		}

		HWND hTask = 0;

		auto thrloop = [&]()
			{
				for (;;)
				{
					if (!MyStreamX)
						break;
					PACKET p;
					int r = rrecv(X, (char*)&p, sizeof(PACKET));
					if (r != sizeof(PACKET))
						break;
					std::vector<char> extra;
					if (p.w != 'G' && p.w != 'P')
					{
						if (p.bytes)
						{
							extra.resize(p.bytes);
							r = rrecv(X, extra.data(), (int)p.bytes);
							if (r != p.bytes)
								break;
							if (p.w == 1 && str)
							{
								// Write request from client's Transport
								str->DataReady(extra);

							}
							if (p.w == 0)
							{
								const wchar_t* key = (const wchar_t*)extra.data();
								if (wcslen(key))
								{
									std::thread tx(&RDP::CreateNewViewer, this, X, std::wstring(key), p.w == 2 ? str : nullptr, true);
									tx.detach();
								}
							}
						}
					}
				}
				if (hTask)
					PostMessage(hTask, TDM_CLICK_BUTTON, IDCANCEL, 0);
			};

		
		std::shared_ptr<std::thread> tx;
		if (!ForReverse && !key)
			tx = std::make_shared<std::thread>(thrloop);


		if (key && !X)
		{
			// Standalone server
			TASK_PARAMS tp;
			tp.cbparam = (LPARAM)&hTask;
			tp.cb = [](HWND hwnd, UINT uNotification, WPARAM wParam, LPARAM lParam, LPARAM dwRefData, LPARAM cbf) -> HRESULT
				{
					if (uNotification == TDN_CREATED)
					{
						HWND* p = (HWND*)cbf;
						*p = hwnd;
					}
					return E_ABORT;
				};
			tp.Marq = false;
			tp.hIcon = hIcon;
			tp.inst = L"RDP Server Active";
			tp.cont = L"Waiting for connections";
			tp.usebuttons.push_back({ 222,L"Copy key to clipboard" });
#ifdef HMN_ENABLE_SERVICE
//			tp.usebuttons.push_back({ 203,L"Service" });
#endif
			tp.usebuttons.push_back({ 202,L"FPS Setting" });
			tp.usebuttons.push_back({ IDCANCEL,L"Close" });
			tp.cmdf = [&](HWND hTask, int cmd)
				{
					if (cmd == 222)
					{
						if (key)
						{
							std::wstring key2 = *key;
							// Key to turbo-play.com
#ifdef HMN_KEY_TURBO_PLAY
	// Uplaod ticket
							RESTAPI::REST r(L"RDP Library");
							r.Connect(L"www.turbo-play.com", true);
							std::string t = XML3::XMLU(key->c_str()).bc();

							auto hi = r.RequestWithBuffer(L"/rdp.php", L"POST", { L"X-Function: createX" }, (const char*)t.data(), t.size());
							std::vector<char> d;
							r.ReadToMemory(hi, d);
							if (d.size() > 0)
							{
								d.resize(d.size() + 1);
								key2 = XML3::XMLU(d.data()).wc();
							}
#else
#endif



							if (OpenClipboard(hTask))
							{
								EmptyClipboard();
								HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, (key2.length() + 1) * 2);
								if (hg)
								{
									wchar_t* p = (wchar_t*)GlobalLock(hg);
									if (p)
									{
										wcscpy_s(p, key2.length() + 1, key2.c_str());
										GlobalUnlock(hg);
										SetClipboardData(CF_UNICODETEXT, hg);
									}
								}
								CloseClipboard();
							}
						}
						return S_FALSE;
					}
					if (cmd == 203)
					{
						HMENU hm = CreatePopupMenu();
						AppendMenu(hm, MF_STRING, 301, L"Install");
						AppendMenu(hm, MF_STRING, 302, L"Remove");
						POINT pt;
						GetCursorPos(&pt);
						int cmd3 = TrackPopupMenu(hm, TPM_RETURNCMD | TPM_NONOTIFY, pt.x, pt.y, 0, hTask, 0);
						DestroyMenu(hm);
						if (cmd3 == 301)
						{
							std::vector<wchar_t> fn(1000);
							GetModuleFileName(0, fn.data(), 1000);
							wchar_t params[1000] = {};
							swprintf_s(params, 1000, L"--installservicerdp -p %i",Port);
							ShellExecute(0, L"runas", fn.data(), params, 0, SW_SHOWNORMAL);
							return S_OK;

						}
						if (cmd3 == 302)
						{
							std::vector<wchar_t> fn(1000);
							GetModuleFileName(0, fn.data(), 1000);
							ShellExecute(0, L"runas", fn.data(), L"--uninstallservicerdp", 0, SW_SHOWNORMAL);
						}
						return S_FALSE;
					}
					if (cmd == 202)
					{
						HMENU hm = CreatePopupMenu();
						AppendMenu(hm, MF_STRING, 301, L"Lowest");
						AppendMenu(hm, MF_STRING, 302, L"Low");
						AppendMenu(hm, MF_STRING, 303, L"Medium");
						AppendMenu(hm, MF_STRING, 304, L"High");
						AppendMenu(hm, MF_STRING, 305, L"Highest");
						POINT pt;
						GetCursorPos(&pt);
						int cmd3 = TrackPopupMenu(hm, TPM_RETURNCMD | TPM_NONOTIFY, pt.x, pt.y, 0, hTask, 0);
						DestroyMenu(hm);
						RDP::PROPERTIES props;
						if (cmd3 == 305)
							props.FrameCaptureIntervalInMs = 33;
						if (cmd3 == 304)
							props.FrameCaptureIntervalInMs = 90;
						if (cmd3 == 303)
							props.FrameCaptureIntervalInMs = 150;
						if (cmd3 == 302)
							props.FrameCaptureIntervalInMs = 400;
						if (cmd3 == 301)
							props.FrameCaptureIntervalInMs = 500;
						srv.Props(props);
						return S_FALSE;
					}
					if (cmd == IDCANCEL)
					{
#ifdef HMN_KEY_TURBO_PLAY
						// Uplaod ticket
						RESTAPI::REST r2(L"RDP Library");
						r2.Connect(L"www.turbo-play.com", true);
						auto hi = r2.RequestWithBuffer(L"/rdp.php", L"POST", { L"X-Function: delete"}, 0, 0);
						std::vector<char> d;
						r2.ReadToMemory(hi, d);
#endif
						return S_OK;
					}
					return S_FALSE;
				};
			tp.param = &srv;
			Task(tp);
			if (str)
				str->Release();
			return;
		}

		TASK_PARAMS tp;
		tp.cbparam = (LPARAM) & hTask;
		tp.cb = [](HWND hwnd, UINT uNotification, WPARAM wParam, LPARAM lParam, LPARAM dwRefData,LPARAM cbf) -> HRESULT
			{
				if (uNotification == TDN_CREATED)
				{
					HWND* p = (HWND*)cbf;
					*p = hwnd;
				}
				return E_ABORT;
			};
		tp.Marq = false;
		tp.hIcon = hIcon;
		tp.inst = L"Help me now is Active";
		tp.cont = L"Keep this active";
#ifdef HMN_ENABLE_SERVICE
		tp.usebuttons.push_back({ 203,L"Service" });
#endif
		tp.usebuttons.push_back({ 201,L"Admin" });
		tp.usebuttons.push_back({ 202,L"FPS Setting" });
		tp.usebuttons.push_back({ IDCANCEL,L"Close" });
		tp.cmdf = [&](HWND hTask, int cmd)
			{
				if (cmd == 203)
				{
					HMENU hm = CreatePopupMenu();
					AppendMenu(hm, MF_STRING, 301, L"Install");
					AppendMenu(hm, MF_STRING, 302, L"Remove");
					POINT pt;
					GetCursorPos(&pt);
					int cmd3 = TrackPopupMenu(hm, TPM_RETURNCMD | TPM_NONOTIFY, pt.x, pt.y, 0, hTask, 0);
					DestroyMenu(hm);
					if (cmd3 == 301)
					{
						std::vector<wchar_t> fn(1000);
						GetModuleFileName(0, fn.data(), 1000);
						ShellExecute(0, L"runas", fn.data(), L"--installservice", 0, SW_SHOWNORMAL);

					}
					if (cmd3 == 302)
					{
						std::vector<wchar_t> fn(1000);
						GetModuleFileName(0, fn.data(), 1000);
						ShellExecute(0, L"runas", fn.data(), L"--uninstallservice", 0, SW_SHOWNORMAL);
					}
					return S_FALSE;
				}
				if (cmd == 201)
				{
					std::vector<wchar_t> fn(1000);
					GetModuleFileName(0, fn.data(), 1000);
					ShellExecute(0, L"runas", fn.data(), 0, 0, SW_SHOWNORMAL);
					return S_OK;
				}
				if (cmd == 202)
				{
					HMENU hm = CreatePopupMenu();
					AppendMenu(hm, MF_STRING, 301, L"Lowest");
					AppendMenu(hm, MF_STRING, 302, L"Low");
					AppendMenu(hm, MF_STRING, 303, L"Medium");
					AppendMenu(hm, MF_STRING, 304, L"High");
					AppendMenu(hm, MF_STRING, 305, L"Highest");
					POINT pt;
					GetCursorPos(&pt);
					int cmd3 = TrackPopupMenu(hm, TPM_RETURNCMD | TPM_NONOTIFY, pt.x, pt.y, 0, hTask, 0);
					DestroyMenu(hm);
					RDP::PROPERTIES props;
					if (cmd3 == 305)
						props.FrameCaptureIntervalInMs = 33;
					if (cmd3 == 304)
						props.FrameCaptureIntervalInMs = 90;
					if (cmd3 == 303)
						props.FrameCaptureIntervalInMs = 150;
					if (cmd3 == 302)
						props.FrameCaptureIntervalInMs = 400;
					if (cmd3 == 301)
						props.FrameCaptureIntervalInMs = 500;
					srv.Props(props);
					return S_FALSE;
				}
				if (cmd == IDCANCEL)
				{
					return S_OK;
				}
				return S_FALSE;
			};
		tp.param = &srv;
		Task(tp);
		if (X)
			closesocket(X);
		if (tx)
			tx->join();
		if (str)
			str->Release();
	}

	HRESULT INeedHelp(const char* hn, int port,bool UseStream)
	{
		SOCKET X = 0;
		WaitTask([&](HWND hTask, void* lp, void* param)
			{
				X = ConnectTo(hn, port);
				SendMessage(hTask, TDM_CLICK_BUTTON, IDCANCEL, 0);
			}, 0);
		if (!X)
			return E_FAIL;

		// Succeed, create a new server and pass it
		std::thread tx(&RDP::CreateANewServer, this, X,UseStream,nullptr,0,nullptr);
		tx.join();

		return S_OK;
	}
	void CreateNewViewer(SOCKET Y,std::wstring key,MyStream* ms,bool ForReverse)
	{
		CoInitialize(0);
		PROPERTIES props;
		if (ms)
			props.SetNetworkStream = ms;
		VIEWER viewer(this,ms,Y);
		viewer.Props(props);
#ifdef HMN_KEY_TURBO_PLAY
		if (wcslen(key.c_str()) > 0 && key[0] != '<')
		{
			// 9-digit key
			RESTAPI::REST r2(L"RDP Library");
			r2.Connect(L"www.turbo-play.com", true);
			std::string k = XML3::XMLU(key.c_str()).bc();
			auto hi = r2.RequestWithBuffer(L"/rdp.php", L"POST", { L"X-Function: get" }, k.data(), k.size());
			std::vector<char> d;
			r2.ReadToMemory(hi, d);
			if (d.empty())
				return;
			d.resize(d.size() + 1);
			key = XML3::XMLU(d.data()).wc();
		}
#endif
		viewer.Connect(_bstr_t(key.c_str()), L"", L"");
		viewer.View(hIcon, L"RDP",ForReverse);
		if (!ForReverse)
			closesocket(Y);
	}

	void AcceptThread(SOCKET Y)
	{
#ifndef _DEBUG
		Beep(440, 250);
		Beep(554, 250);
		Beep(659, 250);
		Beep(554, 250);
		Beep(440, 500);
#endif

		auto str = new MyStream(this,Y);

		for (;;)
		{
			PACKET p;
			int r = rrecv(Y, (char*)&p, sizeof(PACKET));
			if (r != sizeof(PACKET))
				break;

			std::vector<char> extra;
			if (p.w != 'G' && p.w != 'P')
			{
				if (p.bytes)
				{
					extra.resize(p.bytes);
					r = rrecv(Y, extra.data(), (int)p.bytes);
					if (r != p.bytes)
						break;
					if (p.w == 0 || p.w == 2)
					{
						const wchar_t* key = (const wchar_t*)extra.data();
						if (wcslen(key))
						{
							std::thread tx(&RDP::CreateNewViewer, this, Y,std::wstring(key),p.w == 2 ? str : nullptr,false);
							tx.detach();
						}
					}
					if (p.w == 1)
					{
						// Write request from client's Transport
						MessageBeep(0);
						str->DataReady(extra);
					}
				}
			}

		}

		closesocket(Y);
	}


	typedef HRESULT (__stdcall * ev_cb)(void*, DISPID, DISPPARAMS*);
	class MyRDPSessionEvents : public _IRDPSessionEvents
	{
	private:

		int refNum = 0;
		void* lp = 0;
		ev_cb f;


	public:


		CComPtr<IConnectionPointContainer> icpc = 0;
		CComPtr<IConnectionPoint> icp = 0;
		DWORD tid = 0;
		MyRDPSessionEvents(IUnknown* conn,ev_cb x,void* e)
		{
			f = x;
			lp = e;

			conn->QueryInterface(__uuidof(IConnectionPointContainer), (void**)&icpc);
			if (icpc)
			{
				icpc->FindConnectionPoint(__uuidof(_IRDPSessionEvents), &icp);
				if (icp)
				{
					icp->Advise(this, &tid);
				}
			}
		}

		~MyRDPSessionEvents()
		{
			if (icp)
				icp->Unadvise(tid);
		}

		// IUnknown
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(
			/* [in] */ REFIID iid,
			/* [iid_is][out] */ __RPC__deref_out void __RPC_FAR* __RPC_FAR* ppvObject)
		{
			*ppvObject = 0;
			if (iid == IID_IUnknown || iid == IID_IDispatch || iid == __uuidof(_IRDPSessionEvents))
				*ppvObject = this;
			if (*ppvObject)
			{
				((IUnknown*)(*ppvObject))->AddRef();
				return S_OK;
			}
			return E_NOINTERFACE;
		}

		virtual ULONG STDMETHODCALLTYPE AddRef(void)
		{
			refNum++;
			return refNum;
		}

		virtual ULONG STDMETHODCALLTYPE Release(void)
		{
			refNum--;
			if (!refNum)
			{
				delete this;
				return 0;
			}
			return refNum;
		}


		// IDispatch
		virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(
			/* [out] */ __RPC__out UINT* pctinfo)
		{
			return E_NOTIMPL;
		}

		virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(
			/* [in] */ UINT iTInfo,
			/* [in] */ LCID lcid,
			/* [out] */ __RPC__deref_out_opt ITypeInfo** ppTInfo)
		{
			return E_NOTIMPL;
		}

		virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(
			/* [in] */ __RPC__in REFIID riid,
			/* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR* rgszNames,
			/* [range][in] */ UINT cNames,
			/* [in] */ LCID lcid,
			/* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID* rgDispId)
		{
			return E_NOTIMPL;
		}

		virtual /* [local] */ HRESULT STDMETHODCALLTYPE Invoke(
			/* [in] */ DISPID dispIdMember,
			/* [in] */ REFIID riid,
			/* [in] */ LCID lcid,
			/* [in] */ WORD wFlags,
			/* [out][in] */ DISPPARAMS* pDispParams,
			/* [out] */ VARIANT* pVarResult,
			/* [out] */ EXCEPINFO* pExcepInfo,
			/* [out] */ UINT* puArgErr)
		{
			return f(lp,dispIdMember,pDispParams);
		}
	};

	struct PROPERTIES
	{
		std::optional<bool> DrvConAttach;
		std::optional<int> PortId;
		std::optional<int> PortProtocol;
		std::optional<IUnknown*> SetNetworkStream;
		std::optional<IUnknown*> SetClipboardRedirectCallback;
		std::optional<bool> EnforceStrongEncryption;
		std::optional<int> FrameCaptureIntervalInMs;
		std::optional<CTRL_LEVEL> DefaultAttendeeControlLevel;
		std::optional<bool> EnableClipboardRedirect;
		std::optional<bool> EnabledTransports;

		HRESULT Put(IRDPSRAPISessionProperties* p)
		{
			HRESULT hr = 0;
			if (DrvConAttach)
			{
				VARIANT vt1;
				vt1.vt = VT_BOOL;
				vt1.boolVal = *DrvConAttach ? VARIANT_TRUE : VARIANT_FALSE;
				hr = p->put_Property(_bstr_t(L"DrvConAttach"), vt1);
			}

			if (PortId)
			{
				VARIANT vt1;
				vt1.vt = VT_I4;
				vt1.lVal = *PortId;
				hr = p->put_Property(_bstr_t(L"PortId"), vt1);
			}

			if (PortProtocol)
			{
				VARIANT vt1;
				vt1.vt = VT_I4;
				vt1.lVal = *PortProtocol;
				hr = p->put_Property(_bstr_t(L"PortProtocol"), vt1);
			}

			if (SetNetworkStream)
			{
				VARIANT vt1;
				vt1.vt = VT_UNKNOWN;
				vt1.punkVal = *SetNetworkStream;
				hr = p->put_Property(_bstr_t(L"SetNetworkStream"), vt1);
			}

			if (SetClipboardRedirectCallback)
			{
				VARIANT vt1;
				vt1.vt = VT_UNKNOWN;
				vt1.punkVal = *SetClipboardRedirectCallback;
				hr = p->put_Property(_bstr_t(L"SetClipboardRedirectCallback"), vt1);
			}

			if (EnforceStrongEncryption)
			{
				VARIANT vt1;
				vt1.vt = VT_BOOL;
				vt1.boolVal = *EnforceStrongEncryption ? VARIANT_TRUE : VARIANT_FALSE;
				hr = p->put_Property(_bstr_t(L"EnforceStrongEncryption"), vt1);
			}

			if (FrameCaptureIntervalInMs)
			{
				VARIANT vt1;
				vt1.vt = VT_I4;
				vt1.lVal = *FrameCaptureIntervalInMs;
				hr = p->put_Property(_bstr_t(L"FrameCaptureIntervalInMs"), vt1);
			}

			if (DefaultAttendeeControlLevel)
			{
				VARIANT vt1;
				vt1.vt = VT_I4;
				vt1.lVal = *DefaultAttendeeControlLevel;
				hr = p->put_Property(_bstr_t(L"DefaultAttendeeControlLevel"), vt1);
			}

			if (EnableClipboardRedirect)
			{
				VARIANT vt1;
				vt1.vt = VT_BOOL;
				vt1.boolVal = *EnableClipboardRedirect ? VARIANT_TRUE : VARIANT_FALSE;
				hr = p->put_Property(_bstr_t(L"EnableClipboardRedirect"), vt1);
			}

			if (EnabledTransports)
			{
				VARIANT vt1;
				vt1.vt = VT_BOOL;
				vt1.boolVal = *EnabledTransports ? VARIANT_TRUE : VARIANT_FALSE;
				hr = p->put_Property(_bstr_t(L"EnabledTransports"), vt1);
			}

			return hr;
		}

	};

	class VIEWER
	{
	public:
		CComPtr<IRDPSRAPIViewer> viewer;
		CComPtr<MyRDPSessionEvents> events;
		RDP* rdp = 0;
		MyStream* str = 0;
		SOCKET Y = 0;

		VIEWER(RDP* r,MyStream* _str,SOCKET y)
		{
			Y = y;
			rdp = r;
			str = _str;
			viewer.CoCreateInstance(__uuidof(RDPViewer));
			events = new MyRDPSessionEvents(viewer,[](void* lp, DISPID dispIdMember, DISPPARAMS* pDispParams) -> HRESULT
				{
//					VIEWER* v = (VIEWER*)lp;
					LogEvent("CLIENT",dispIdMember);

					if (dispIdMember == DISPID_RDPSRAPI_EVENT_ON_ATTENDEE_DISCONNECTED)
					{
						MessageBeep(0);
					}
					return S_OK;
				}, this);
		}

		void Props(PROPERTIES& p)
		{
			CComPtr< IRDPSRAPISessionProperties> props;
			viewer->get_Properties(&props);
			if (props)
				p.Put(props);
		}


		void RequestControl(int cl)
		{
			viewer->RequestControl((CTRL_LEVEL)cl);
		}


		void RequestColor(int cl)
		{
			viewer->RequestColorDepthChange(cl);
		}


		void SmartResize(bool x)
		{
			viewer->put_SmartSizing(x ? VARIANT_TRUE : VARIANT_FALSE);
		}


		bool Connect(const wchar_t* str5, const wchar_t* name, const wchar_t* pwd)
		{
			HRESULT hr = viewer->Connect(_bstr_t(str5), _bstr_t(name), _bstr_t(pwd));
			if (FAILED(hr))
				return false;
			return true;
		}
		bool Disconnect()
		{
			HRESULT hr = viewer->Disconnect();
			if (FAILED(hr))
				return false;
			return true;
		}


		struct VIEW_STRUCT
		{
			MyStream* str = 0;
			VIEWER* viewer = 0;
		};
		static LRESULT CALLBACK Main_DP(HWND hh, UINT mm, WPARAM ww, LPARAM ll)
		{
			if (mm == WM_CREATE)
			{
				CREATESTRUCT* cs = (CREATESTRUCT*)ll;
				SetWindowLongPtr(hh, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams);
				return 0;
			}
			VIEW_STRUCT* v = (VIEW_STRUCT*)GetWindowLongPtr(hh, GWLP_USERDATA);
			if (!v)
				return DefWindowProc(hh, mm, ww, ll);
			return v->viewer->Main2_DP(hh, mm, ww, ll,v);
		}

		LRESULT CALLBACK Main2_DP(HWND hh, UINT mm, WPARAM ww, LPARAM ll,VIEW_STRUCT* v)
		{
			switch (mm)
			{

			case WM_CLOSE:
				{
					viewer->Disconnect();
					DestroyWindow(GetDlgItem(hh, 888));
					DestroyWindow(hh);
					return 0;
				}

				case WM_DESTROY:
				{
					PostQuitMessage(0);
					return 0;
				}

				case WM_SIZE:
				{
				RECT rc;
				GetClientRect(hh, &rc);
				HWND hQ = GetDlgItem(hh, 888);
				SetWindowPos(hQ, 0, 0, 0, rc.right, rc.bottom, SWP_SHOWWINDOW);
				}


				case WM_COMMAND:
				{
					int LW = LOWORD(ww);
					if (LW == 101)
						SmartResize(true);
					if (LW == 102)
						SmartResize(false);
					if (LW == 103)
						RequestControl(CTRL_LEVEL_VIEW);
					if (LW == 104)
						RequestControl(CTRL_LEVEL_INTERACTIVE);
					if (LW == 106)
						RequestControl(CTRL_LEVEL_REQCTRL_INTERACTIVE);
					if (LW == 105)
						RequestControl(CTRL_LEVEL_NONE);
					if (LW == 201)
					{
						std::thread tx(&RDP::CreateANewServer, v->viewer->rdp,v->viewer->Y, false,nullptr,v->viewer->rdp->reverse_port,nullptr);
						tx.detach();
					}
					if (LW == 202)
					{
						OPENFILENAME of = { 0 };
						of.lStructSize = sizeof(of);
						of.hwndOwner = 0;
						of.lpstrFilter = L"*.*\0*.*\0\0";
						of.lpstrInitialDir = 0;
						of.nFilterIndex = 0;
						wchar_t fnx[10000] = { 0 };;
						of.lpstrFile = fnx;
						of.nMaxFile = 10000;
						of.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_EXPLORER;
						if (!GetOpenFileName(&of))
							return 0;

					}
					return 0;
				}

			}
			return DefWindowProc(hh, mm, ww, ll);
		}

		void View(HICON hIcon,const wchar_t* wn,bool ForReverse)
		{
			WNDCLASSEX wClass = { 0 };
			wClass.cbSize = sizeof(wClass);

			wClass.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_PARENTDC;
			wClass.lpfnWndProc = (WNDPROC)Main_DP;
			wClass.hInstance = GetModuleHandle(0);
			wClass.hIcon = hIcon;
			wClass.hCursor = LoadCursor(0, IDC_ARROW);
			wClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			wClass.lpszClassName = _T("{1A7CDA67-377E-4D4C-B4DA-7E6C6B857EA6}");
			wClass.hIconSm = hIcon;
			RegisterClassEx(&wClass);

			// The Menu
			HMENU hM = CreateMenu();
//				HMENU hM0 = CreatePopupMenu();

			HMENU hM1 = CreatePopupMenu();
			AppendMenu(hM1, MF_STRING, 101, L"Screen Scale on");
			AppendMenu(hM1, MF_STRING, 102, L"Screen Scale off");
			AppendMenu(hM, MF_POPUP | MF_STRING, (UINT_PTR)hM1, L"Scaling");
			if (1)
			{
				HMENU hM2 = CreatePopupMenu();
				AppendMenu(hM2, MF_STRING, 103, L"View");
				AppendMenu(hM2, MF_STRING, 104, L"Interact");
				AppendMenu(hM2, MF_STRING, 106, L"Q Interact");
				AppendMenu(hM2, MF_STRING, 105, L"Off");
				AppendMenu(hM, MF_POPUP | MF_STRING, (UINT_PTR)hM2, L"Interact mode");
			}
			if (!ForReverse)
			{
				HMENU hM3 = CreatePopupMenu();
				AppendMenu(hM3, MF_STRING, 201, L"Start Reverse");
//				AppendMenu(hM3, MF_STRING, 202, L"Send file");
				AppendMenu(hM, MF_POPUP | MF_STRING, (UINT_PTR)hM3, L"Other");
			}
			// Create Window
			VIEW_STRUCT vs;
			vs.str = str;
			vs.viewer = this;
			HWND hh = CreateWindowEx(0,
				_T("{1A7CDA67-377E-4D4C-B4DA-7E6C6B857EA6}"),
				wn,
				WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_VISIBLE |
				WS_CLIPCHILDREN, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
				0, hM, GetModuleHandle(0), &vs);

			HWND hV = CreateWindowEx(0, L"AX_RDP", L"}32BE5ED2-5C86-480F-A914-0FF8885A1B3F}", WS_CHILD | WS_VISIBLE, 0, 0, 1, 1, hh, (HMENU)888, 0, 0);
			IRDPSRAPIViewer* v = viewer.p;
			SendMessage(hV, AX_RECREATE, 0, (LPARAM)v);
			SendMessage(hV, AX_INPLACE, 1, 0);
			SendMessage(hh, WM_SIZE, 0, 0);
			MSG msg;
			while (GetMessage(&msg, 0, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

	};



	class SERVER
	{
	public:

		CComPtr<IRDPSRAPISharingSession> server;
		CComPtr<MyRDPSessionEvents> events;

		SERVER()
		{
			server.CoCreateInstance(__uuidof(RDPSession));
			events = new MyRDPSessionEvents(server,[](void* lp, DISPID dispIdMember, DISPPARAMS* pDispParams) -> HRESULT
				{
					SERVER* v = (SERVER*)lp;
					LogEvent("SERVER", dispIdMember);


					if (dispIdMember == DISPID_RDPSRAPI_EVENT_ON_ATTENDEE_CONNECTED)
					{
						auto d = GetDisp(pDispParams);
						if (!d)
							return S_OK;
						CComPtr<IRDPSRAPIAttendee> at;
						d.QueryInterface<IRDPSRAPIAttendee>(&at);
						if (!at)
							return S_OK;
						at->put_ControlLevel(CTRL_LEVEL_VIEW);
						CComBSTR b;
						at->get_RemoteName(&b);
					}

					if (dispIdMember == DISPID_RDPSRAPI_EVENT_ON_CTRLLEVEL_CHANGE_REQUEST)
					{
						auto d = GetDisp(pDispParams);
						if (!d)
							return S_OK;
						CComPtr<IRDPSRAPIAttendee> at;
						d.QueryInterface<IRDPSRAPIAttendee>(&at);
						if (!at)
							return S_OK;
						CTRL_LEVEL le = (CTRL_LEVEL)pDispParams->rgvarg[0].intVal;


						//bool IsBack = 0;
/*						auto s = v;
						if (s)
						{
						}
						else
						{
							s = q->s2;
							IsBack = 1;
							if (le == CTRL_LEVEL_INTERACTIVE || CTRL_LEVEL_REQCTRL_INTERACTIVE)
							{
								if (MessageBox(0, L"Switch to interactive BackView?", L"RDP", MB_YESNO | MB_SYSTEMMODAL) == IDNO)
									return S_OK;
							}
						}
						*/

						at->put_ControlLevel(le);

						CComBSTR b;
						at->get_RemoteName(&b);
						if (b)
						{
						}
						//Sharedstuff
						if (v)
						{
							std::vector<int> app_pids;
							std::vector<std::wstring> app_names;
							std::vector<int> app_st;
							v->GetShareableApplications(app_pids, app_names, app_st);
							for (size_t i = 0; i < app_pids.size(); i++)
							{
							}
							v->ShareOnlyTheseApplications(app_pids, true);

							CComPtr<IRDPSRAPIApplicationFilter> af = 0;
							v->server->get_ApplicationFilter(&af);
							if (af)
							{
								af->put_Enabled(VARIANT_FALSE);
							}
						}
					}

					return S_OK;
				}, this);


		}

		void Props(PROPERTIES& p)
		{
			CComPtr< IRDPSRAPISessionProperties> props;
			server->get_Properties(&props);
			if (props)	
				p.Put(props);
			if (p.SetNetworkStream)
			{
				CComPtr<IRDPSRAPISharingSession2> server2;
				server2 = server;
				if (server2)
				{
					auto hr = server2->ConnectUsingTransportStream((IRDPSRAPITransportStream*)*p.SetNetworkStream,_bstr_t(L"HMN"),_bstr_t(L"HMN"));
					if (FAILED(hr))
						MessageBeep(0);
				}

			}
		}

		HRESULT Open(const wchar_t* rev)
		{
			if (rev)
				return server->ConnectToClient(_bstr_t(rev));
			else
				return server->Open();
		}
		CComPtr<IRDPSRAPIInvitation>  GetInvitation(const wchar_t* str1 = 0, const wchar_t* pwd = L"", const wchar_t* grp = L"RDP", int limit = 10)
		{
			CComPtr<IRDPSRAPIInvitationManager> invm;
			server->get_Invitations(&invm);
			if (invm)
			{
				CComPtr<IRDPSRAPIInvitation> inv;
				invm->CreateInvitation((str1 == nullptr) ? (BSTR)_bstr_t(str1) : (BSTR)nullptr, _bstr_t(grp), _bstr_t(pwd), limit, &inv);
				return inv;
			}
			return nullptr;
		}

		void Close()
		{
			if (server)
				server->Close();
		}
		~SERVER()
		{
			Close();
			server = 0;
		}

		void Pause()
		{
			server->Pause();
		}

		void Resume()
		{
			server->Resume();
		}
		bool SetDesktopRegion(const RECT& rc)
		{
			server->SetDesktopSharedRect(rc.left, rc.top, rc.right, rc.bottom);
			return true;
		}
		bool GetDesktopRegion(RECT& rc)
		{
			server->GetDesktopSharedRect(&rc.left, &rc.top, &rc.right, &rc.bottom);
			return true;
		}
		bool SetApplicationState(int pid, bool GlobalShare)
		{
			bool G = GlobalShare;
			return SetGetApplicationState(pid, 1, G);
		}
		void GetShareableApplications(std::vector<int>& pids, std::vector<std::wstring>& names, std::vector<int>& ST)
		{
			// Get the application pids list
			std::vector<CComVariant> v;
			CComPtr<IRDPSRAPIApplicationFilter> af = 0;
			server->get_ApplicationFilter(&af);
			if (af)
			{
				CComPtr<IRDPSRAPIApplicationList> afl = 0;
				af->get_Applications(&afl);
				if (afl)
				{
					CComPtr<IUnknown> ei = 0;
					afl->get__NewEnum(&ei);
					if (ei)
						GetEnumerationC<IEnumVARIANT>(ei, v);
				}
			}
			for (unsigned int i = 0; i < v.size(); i++)
			{
				CComPtr<IRDPSRAPIApplication> app = 0;
				if (!v[i].pdispVal)
					continue;
				v[i].pdispVal->QueryInterface(__uuidof(IRDPSRAPIApplication), (void**)&app);
				if (!app)
					continue;
				long lid = 0;
				CComBSTR bx = 0;
				VARIANT_BOOL bz;
				app->get_Id(&lid);
				app->get_Name(&bx);
				app->get_Shared(&bz);
				pids.push_back(lid);
				names.push_back(std::wstring(bx ? (std::wstring)bx : (std::wstring)L""));
				ST.push_back(bz == VARIANT_TRUE ? 1 : 0);
			}
		}

		void ShareAllApplications()
		{
			std::vector<CComVariant> v;
			CComPtr<IRDPSRAPIApplicationFilter> af = 0;
			server->get_ApplicationFilter(&af);
			if (af)
				af->put_Enabled(VARIANT_FALSE);
			return;
		}

		void ShareOnlyTheseApplications(std::vector<int>& pids, bool X)
		{
			// Get the application pids list
			std::vector<CComVariant> v;
			CComPtr<IRDPSRAPIApplicationFilter> af = 0;
			server->get_ApplicationFilter(&af);
			if (af)
			{
				af->put_Enabled(X ? VARIANT_TRUE : VARIANT_FALSE);

				CComPtr<IRDPSRAPIApplicationList> afl = 0;
				af->get_Applications(&afl);
				if (afl)
				{
					CComPtr<IUnknown> ei = 0;
					afl->get__NewEnum(&ei);
					if (ei)
						GetEnumerationC<IEnumVARIANT>(ei, v);
				}
			}

			for (unsigned int i = 0; i < v.size(); i++)
			{
				CComPtr<IRDPSRAPIApplication> app = 0;
				if (!v[i].pdispVal)
					continue;
				v[i].pdispVal->QueryInterface(__uuidof(IRDPSRAPIApplication), (void**)&app);
				if (!app)
					continue;

				HRESULT hr = 0;
				if (!X)
					app->put_Shared(VARIANT_TRUE);
				else
				{
					long xpid = 0;
					app->get_Id(&xpid);

					// Is it there?
					bool Found = false;
					for (unsigned int jjj = 0; jjj < pids.size(); jjj++)
					{
						if (pids[jjj] == xpid)
						{
							Found = true;
							break;
						}
						if (pids[jjj] == 0 && xpid == (long)GetCurrentProcessId())
						{
							Found = true;
							break;
						}
					}

					if (Found)
					{
						hr = app->put_Shared(VARIANT_TRUE);
					}
					else
					{
						hr = app->put_Shared(VARIANT_FALSE);
						BSTR bn = 0;
						app->get_Name(&bn);
						SysFreeString(bn);

						// All windows non shared
						CComPtr<IRDPSRAPIWindowList> wl = 0;
						app->get_Windows(&wl);
						if (wl)
						{
							CComPtr<IUnknown> ei = 0;
							wl->get__NewEnum(&ei);
							if (ei)
							{
								std::vector<CComVariant> v2;
								GetEnumerationC<IEnumVARIANT>(ei, v2);
								for (unsigned int jk = 0; jk < v2.size(); jk++)
								{
									CComPtr<IRDPSRAPIWindow> wi = 0;
									if (!v2[jk].pdispVal)
										continue;
									v2[jk].pdispVal->QueryInterface(__uuidof(IRDPSRAPIWindow), (void**)&wi);
									if (wi)
										wi->put_Shared(VARIANT_FALSE);
								}
							}
						}
					}
				}
			}
		}

		bool GetApplicationState(int pid, bool* GlobalShare)
		{
			if (!GlobalShare)
				return false;

			bool G = false;
			bool X = SetGetApplicationState(pid, 0, G);
			if (X)
				*GlobalShare = G;
			return X;
		}

		bool SetGetApplicationState(int pid, int sg, bool& GlobalShare)
		{
			if (pid == 0)
				pid = GetCurrentProcessId();

			// Add the application
			CComPtr<IRDPSRAPIApplicationFilter> af = 0;
			server->get_ApplicationFilter(&af);
			bool F = false;
			if (af)
			{
				af->put_Enabled(VARIANT_TRUE);
				CComPtr<IRDPSRAPIApplicationList> afl = 0;
				af->get_Applications(&afl);
				if (afl)
				{
					CComPtr<IRDPSRAPIApplication> app = 0;
					afl->get_Item(pid, &app);
					if (app)
					{
						if (sg == 1) // set
						{
							HRESULT hr = app->put_Shared(GlobalShare ? VARIANT_TRUE : VARIANT_FALSE);
							if (SUCCEEDED(hr))
								F = true;
						}
						else // 0 - get
						{
							VARIANT_BOOL bx;
							HRESULT hr = app->get_Shared(&bx);
							if (SUCCEEDED(hr))
							{
								F = true;
								if (bx == VARIANT_TRUE)
									GlobalShare = true;
								else
									GlobalShare = false;
							}
						}
					}
				}
			}
			return F;
		}




	public:
	};


	HANDLE hTerm1 = 0;
	void StartAcceptingControlServerAccept()
	{
		for (;;)
		{
			SOCKET s2 = accept(MainServerSocket,0,0);
			if (s2 == INVALID_SOCKET)
				break;
			std::thread s3(&RDP::AcceptThread, this, s2);
			s3.detach();
		}
		SetEvent(hTerm1);
	}

	SOCKET ConnectTo(const char* addr, int port)
	{
		// Check the address
		if (!addr || !port)
			return 0;

		SOCKET X = socket(AF_INET6, SOCK_STREAM, 0);
		if (X == INVALID_SOCKET)
			return 0;
		DWORD ag = 0;
		setsockopt(X, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&ag, 4);

		wchar_t a[1000] = {};
		swprintf_s(a, 1000, L"%S", addr);
		wchar_t sn[100] = {};
		swprintf_s(sn, 100, L"%u", port);
		if (!WSAConnectByName(X, a, sn, 0, 0, 0, 0, 0, 0))
		{
			closesocket(X);
			return 0;
		}

		return X;
	}




	int reverse_port = 0;
	HRESULT StartAcceptingControlServer(int port,int revport)
	{
		reverse_port = revport;
		DWORD ag = 0;
		MainServerSocket = socket(AF_INET6, SOCK_STREAM, 0);
		setsockopt(MainServerSocket, IPPROTO_IPV6, IPV6_V6ONLY, (char*)&ag, 4);
		sockaddr_in6 sA = { 0 };
		sA.sin6_port = htons((short)port);
		sA.sin6_family = AF_INET6;

		/*				X = socket(AF_INET, SOCK_STREAM, 0);
						sockaddr_in sA = {};
						sA.sin_port = htons((short)TCPPort);
						sA.sin_family = AF_INET;
						*/
		if (::bind(MainServerSocket, (sockaddr*)&sA, sizeof(sA)) < 0)
		{
			closesocket(MainServerSocket);
			MainServerSocket = 0;
			return E_FAIL;
		}
		listen(MainServerSocket, 3);
		hTerm1 = CreateEvent(0, 0, 0, 0);
		std::thread s4(&RDP::StartAcceptingControlServerAccept, this);
		s4.detach();
		return S_OK;
	}



	RDP()
	{
		AXLIBRARY::AXRegister();
	}
	~RDP()
	{
		if (MainServerSocket)
		{
			closesocket(MainServerSocket);
			MainServerSocket = 0;
			if (hTerm1)
			{
				WaitForSingleObject(hTerm1, INFINITE);
				CloseHandle(hTerm1);
				hTerm1 = 0;
			}
		}
	}
};


}; // RDP