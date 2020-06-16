#include "FileManager.h"
#include "User.h"

#define TEXTS	"texts.txt"
#define REPORT	"report.pdf"
#define PHOTO	"photo.jpg"

/*返回这个命令是不是一个合法的命令*/
bool SplitCmd(string line, vector<string> &args) {
	args.clear();
	int preSpacePos = 0;
	int j = 0;
	for (int i = 0; i < line.length(); i++) {
		if (line[i] == ' ') {
			/*如果出现多个连续的空格*/
			if (preSpacePos == i) {
				preSpacePos = i + 1;
				continue;
			}
			if (j >= 4) {
				return false;
			}
			args.push_back(line.substr(preSpacePos, i-preSpacePos));
			preSpacePos = i + 1;
			j++;
		}
	}
	if (preSpacePos != line.length()) {
		args.push_back(line.substr(preSpacePos, line.length()));
	}
	return true;
}

void saveFile(User& user, string filename) {
	ifstream in(filename, ios::binary);
	if (!in) {
		cout << "Please The File " << filename << " exists" << endl;
		return;
	}
	int fd = user.fileManager->fopen(filename, 3);
	bool flag = true;
	int startPlace = 0;
	while (flag) {
		char buf[1025];
		memset(buf, 0, 1025);
		in.read(buf, 1024);
		//in.get(buf, 1024);
		user.fwrite(fd, startPlace, buf, 1024);
		startPlace += 1024;
		if (in.eof()) {
			break;
		}
	}
	user.fclose(fd);
}

void autoRun(User& user) {
	user.fformat();
	user.mkdir("bin");
	user.mkdir("etc");
	user.mkdir("home");
	user.mkdir("dev");
	user.cd("home");
	user.mkdir("texts");
	user.mkdir("reports");
	user.mkdir("photos");
	user.cd("texts");
	user.fcreat(TEXTS);
	saveFile(user, TEXTS);
	user.cd("..");
	user.cd("photos");
	user.fcreat(PHOTO);
	saveFile(user, PHOTO);
	user.cd("../reports");
	user.fcreat(REPORT);

}

void Help() {
	cout << "********************help********************" << endl;

	cout << "auto:" << endl;
	cout << "Usage:" << "To auto Run, create directorys in ppt, and put dile into texts, photos, reports" << endl;
	cout << "form:" << "auto" << endl;
	cout << "example:" << "auto" << endl;
	cout << endl;
	
	cout << "help:" << endl;
	cout << "Usage:" << "To get help Infomation" << endl;
	cout << "form:" << "help" << endl;
	cout << "example:" << "help" << endl;
	cout << endl;


	cout << "fformat:" << endl;
	cout << "Usage:" << "to format all the Disk" << endl;
	cout << "form:" << "fformat" << endl;
	cout << "example:" << "fformat" << endl;
	cout << endl;

	cout << "exit:" << endl;
	cout << "Usage:" << "To quit this application" << endl;
	cout << "form:" << "exit" << endl;
	cout << "example:" << "exit" << endl;
	cout << endl;

	cout << "mkdir:" << endl;
	cout << "Usage:" << "To create a Directory" << endl;
	cout << "form:" << "mkdir --dirname" << endl;
	cout << "example:" << "mkdir bin" << endl;
	cout << endl;

	cout << "cd:" << endl;
	cout << "Usage:" << "To Inside a directory" << endl;
	cout << "form:" << "cd --diranme" << endl;
	cout << "example:" << "\tcd bin" << endl;
	cout << "\tcd ../bin" << endl;
	cout << endl;

	cout << "pwd:" << endl;
	cout << "Usage:" << "To show current Path" << endl;
	cout << "form:" << "pwd" << endl;
	cout << "example:" << "pwd" << endl;
	cout << endl;

	cout << "fcreat:" << endl;
	cout << "Usage:" << "To create a file" << endl;
	cout << "form:" << "fcreat --filename" << endl;
	cout << "example:" << "fcreat test.txt" << endl;
	cout << endl;

	cout << "fopen:" << endl;
	cout << "Usage:" << "To open a file" << endl;
	cout << "form:" << "fopen --filename --openMode" << endl;
	cout << "example:" << "fopen test.txt 3" << endl;
	cout << "\topenMode=3 read And write" << endl;
	cout << "\topenMode=2 read Only" << endl;
	cout << "\topenMode=1 write Only" << endl;
	cout << "\topenMode=0 all disable" << endl;
	cout << "caution:" << "This command output a fd(Integer) to read&write file" << endl;
	cout << endl;

	cout << "fread:" << endl;
	cout << "Usage:" << "To read a file" << endl;
	cout << "form:" << "fread --fd --startReadPlace --len" << endl;
	cout << "example:" << "fread 1 0 10" << endl;
	cout << endl;

	cout << "fwrite:" << endl;
	cout << "Usage:" << "To write a file" << endl;
	cout << "form:" << "fwrite --fd --startWritePlace --len" << endl;
	cout << "example:" << "fwrite 1 0 test" << endl;
	cout << endl;

	cout << "fclose:" << endl;
	cout << "Usage:" << "To clsoe a file" << endl;
	cout << "form:" << "fclose --fd" << endl;
	cout << "example:" << "fclose 1" << endl;
	cout << endl;

	cout << "fdelete:" << endl;
	cout << "Usage:" << "To delete a file" << endl;
	cout << "form:" << "fdelete --filename" << endl;
	cout << "example:" << "test.txt" << endl;
	cout << endl;

	cout << "******************help end******************" << endl;
}

int main() {
	User user("root");
	string cmd, arg1, arg2, arg3;
	string line = "";
	vector<string> args;

	cout << "********************************UNIX FILE System********************************" << endl;
	cout << "[Unix" << user.curDirPath << "@" << user.username << "]$";
	while (1) {
		/*切割命令*/
		char c[100];
		cin.getline(c, 100);
		line = c;
		bool flag = SplitCmd(line, args);
		cmd = args.at(0);
		arg1 = args.size() > 1 ? args.at(1) : "";
		arg2 = args.size() > 2 ? args.at(2) : "";
		arg3 = args.size() > 3 ? args.at(3) : "";
		if (cmd == "exit")
			exit(0);
		else if (cmd == "fformat") {
			/*格式化*/
			user.fformat();
		}
		else if (cmd == "mkdir") {
			user.fileManager->mkdir(arg1);
		}
		else if (cmd == "ls") {
			user.Ls();
		}
		else if (cmd == "cd") {
			user.cd(arg1);
		}
		else if (cmd == "pwd") {
			user.pwd();
		}
		else if (cmd == "fcreat") {
			/*在当前目录下创建一个文件*/
			user.fcreat(arg1);
		}
		else if (cmd == "fopen") {
			/*返回一个文件句柄*/
			user.fopen(arg1, atoi(arg2.data()));
		}
		else if (cmd == "fread") {
			user.fread(atoi(arg1.data()), atoi(arg2.data()), atoi(arg3.data()));
		}
		else if (cmd == "fwrite") {
			user.fwrite(atoi(arg1.data()), atoi(arg2.data()), arg3.data(), arg3.length());
		}
		else if (cmd == "fclose") {
			user.fclose(atoi(arg1.data()));
		}
		else if (cmd == "fdelete") {
			user.fdelete(arg1);
		}
		else if (cmd == "auto") {
			autoRun(user);
		}
		else if (cmd == "help") {
			Help();
		}
		else {
			cout << "No Command Named " << cmd << endl;
			cout << "You can get Help Infomation by using command \"help\"" << endl;
		}
		cout << "[Unix" << user.curDirPath << "@" << user.username << "]$";
	}
	return 0;
}