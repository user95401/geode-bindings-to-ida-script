// geode-bindings-to-ida-script.cpp: определяет точку входа для приложения.
//

#include <Windows.h>
#include <iostream>
#include <format>
#include <regex>
#include <filesystem>

using namespace std;

#include <fstream>
auto read_file(std::string_view path) -> std::string {
    constexpr auto read_size = std::size_t(4096);
    auto stream = std::ifstream(path.data());
    stream.exceptions(std::ios_base::badbit);
    if (not stream) {
        return ("file does not exist");
    }
    auto out = std::string();
    auto buf = std::string(read_size, '\0');
    while (stream.read(&buf[0], read_size)) {
        out.append(buf, 0, stream.gcount());
    }
    out.append(buf, 0, stream.gcount());
    return out;
}
std::vector<std::string> separate(const std::string& s, char aChar)
{
    std::vector<std::string> line;
    std::string token;
    std::istringstream iss(s);
    while (std::getline(iss, token, aChar))
        line.push_back(token);
    return line;
}
void CreateConsole()
{
    if (!AllocConsole()) {
        // Add some error handling here.
        // You can call GetLastError() to get more info about the error.
        return;
    }

    // std::cout, std::clog, std::cerr, std::cin
    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();
};

class Member {
public:
    string m_name = string("undefmember");
    string m_addr = string("undefmember");
    void SetName(string sName) {
        m_name = std::regex_replace(sName, std::regex("\\W"), "");
    }
    void SetAddr(string sAddr) {
        m_addr = (sAddr);
    }
};

class Container {
public:
    string m_name = string("undefclassorns");
    vector<Member*> m_memes;
    void SetName(string sName) {
        m_name = std::regex_replace(sName, std::regex("\\W"), "");
    }
    void AddMember(Member* pMember) {
        m_memes.push_back(pMember);
    }
};

filesystem::path TarFile;
filesystem::path OutFile;
vector<Container*> Containers;

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int show)
{
    //TarFile
	BOOL ReqRtn;
	OPENFILENAME OFN = { 0 };
    // common dialog box structure, setting all fields to 0 is important
    TCHAR szFile[260] = { 0 };
    // Initialize remaining fields of OPENFILENAME structure
    OFN.lStructSize = sizeof(OFN);
    OFN.lpstrFile = szFile;
    OFN.nMaxFile = sizeof(szFile);
    OFN.lpstrFilter = "Broma file\0*.bro\0\0";
    OFN.nFilterIndex = 1;
    OFN.lpstrTitle = "Select broma file to convert (GeometryDash.bro):";
    OFN.nMaxFileTitle = 0;
    OFN.lpstrInitialDir = NULL;
    OFN.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    ReqRtn = GetOpenFileName(&OFN);
	if (ReqRtn == TRUE)
	{
		filesystem::path filename(OFN.lpstrFile);
		TarFile = filename;
	}
	else return 0;
    //OutPath
    auto idafilename = TarFile.filename().string();
    idafilename = std::regex_replace(idafilename, std::regex(".bro"), ".idc");
    OFN.lpstrFile = (TarFile / idafilename).string().data();
    OFN.nFilterIndex = 1;
    OFN.Flags = OFN_PATHMUSTEXIST;
    OFN.lpstrFilter = "IDA Script\0*.idc\0\0";
    OFN.lpstrTitle = "Save output .idc file to:";
    (GetSaveFileName(&OFN));
    OutFile = OFN.lpstrFile;
    if(IDCANCEL == MessageBoxA(0, std::format(
        "TarFile:" "\n"
        "{}" "\n"
        "\n"
        "OutPath:" "\n"
        "{}", TarFile.string(), OutFile.string()
    ).data(), "Continue ya? (Click No to set paths again)", MB_YESNO)) return WinMain(inst, prev, cmd, show);
    //scary things goes herebool attached = AttachConsole(ATTACH_PARENT_PROCESS) != 0;
    CreateConsole();
    auto TarFileStr = read_file(TarFile.string());
    auto lines = separate(TarFileStr, '\n');
    Container* currentClass;
    for (std::string line : lines) {
        if (line.find("class") != std::string::npos) {
            std::string classname;
            for (auto c : line.replace(0, 6, "")) {
                if (c != ' ') classname.push_back(c);
                else if (!classname.empty()) break;
            };
            cout << "class " << classname << endl;
            auto thisClass = new Container;
            thisClass->SetName(classname);
            currentClass = thisClass;
            Containers.push_back(thisClass);
        }
        else if (currentClass and line.find(") = win 0x") != std::string::npos) {
            std::string membername(line);
            {
                auto asd = separate(membername, ' ');
                //"       virtual"[0]"void"[1]"FLAlert_Clicked(FLAlertLayer*,"[2]"bool)"[3]"="[4]"win"[5]"0x96ca0;"[6]
                //"       void"[0]"onBack(cocos2d::CCObject*"[1]"sender)"[2]"="[3]"win"[4]"0x268d00;"[5]
                //remove type
                if (asd[0].find("virtual") != std::string::npos or asd[0].find("static") != std::string::npos) {
                    auto toremove = asd[0] + " " + asd[1];//virt + typename
                    membername.replace(0, toremove.size(), "");
                }
                else {
                    auto toremove = asd[0];//typename
                    membername.replace(0, toremove.size(), "");
                }
                //no space
                membername = std::regex_replace(membername, std::regex(" "), "");
                //remove rest shit
                if (membername.find("(") != string::npos) membername.replace(membername.find("("), membername.size(), "");
            }
            std::string addr(line);
            {
                addr;
                //no space
                addr = std::regex_replace(addr, std::regex(" "), "");
                //remove rest shit
                if (addr.find(")=win") != string::npos) addr.replace(0, addr.find(")=win") + 5, "");
                //remove ";" and comments
                if (addr.find(";") != string::npos) addr.replace(addr.find(";"), addr.size(), "");
            }
            cout << membername << " = " << "\"" << addr << "\"" << endl;
            auto member = new Member;
            member->SetName(membername);
            member->SetAddr(addr);
            currentClass->AddMember(member);
        }
    };
    cout << "\n\n\n\nBROMA FILE PARCED\nNext up is script file gen :D\n\n...MessageBox here\n";
    MessageBox(0, "BROMA FILE PARCED\nNext up is script file gen :D", "geode-bindings-to-ida-script", 0);
    auto OutFileStream = std::ofstream(OutFile.string());
    string names;
    auto openFunc =  
    "static main()\n"
    "{\n""   auto base = get_imagebase();\n";
    OutFileStream << openFunc;
    cout << openFunc;
    for (auto Container : Containers) {
        for (auto Member : Container->m_memes) {
            int namenum = 0;
            auto name = Container->m_name + Member->m_name;
            while (names.find(name) != string::npos) {
                ++namenum;
                name = name + to_string(namenum);
            }
            names = name + (name + ";");
            auto entry = std::format(
                "   set_name(base + {}, \"{}_{}\");\n",
                Member->m_addr, Container->m_name, Member->m_name + (namenum == 0 ? "" : to_string(namenum))
            );
            OutFileStream << entry;
            cout << entry;
        }
    }
    OutFileStream << "}";
    cout << "}";
    return MessageBox(0, "DONE", "geode-bindings-to-ida-script", 0);
}
