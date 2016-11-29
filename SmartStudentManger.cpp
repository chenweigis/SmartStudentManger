
// SmartStudentManger.cpp : 定义控制台应用程序的入口点。
//
/************************************************************************/
/*学生管理系统：
注册->登陆->学生（查询自己相关信息的）
          ->老师（管理员权限）增、删、改、查

1.申请账号时需要填写个人相关信息
2.（管理员账户）教师账户登录时，创建用户信息生成默认账户密码 
账户：学生姓名+学号
密码：123456

StuInfo.txt->保存学生的相关信息         
AccoutInfo.txt->保存账号密码信息

通过链表读取文件信息，进行操作，增删改的时候写入文件，避免多次访问文件，影响系统效率


2016.11.20
Q：注册账号时，通过fwrite()将结构体写入文件之后，字符串末尾为' '(空格)
登账账号通过gets()函数读到的字符串末尾的'\0';
导致登录失败
A:结构体中存在bool型数据为赋值，导致数据存在错误

2016.11.27
bug：
	定义了一个STUNODE*类型的指针s_ModifyStuInfo，指向一个申请到的STUNODE大小空间
	！！！！！！！！！！接着又把这个指针指向了头节点！！！！！！！！！！！！！！！
	在执行完函数之后释放这个指针指向的空间，释放的是！！！！指向的头节点的堆空间！！！！，而不是申请到的堆空间；
	导致到后面再申请STUNODE*类型指针并分配STUNODE大小的堆空间的时候，还是申请到刚刚释放的堆空间
	结果就是头指针与新申请到的指针指向同一块地址；
	 总结说明：
	1.释放指针空间，释放的是指针目前指向的空间，释放时一定要判断清楚，这个指针现在指向哪里！
	2.链表在初始化、增加节点时需要申请空间，其他时候只定义一个链接类型的指针就好！不要分配空间！分配完了还得释放，释放时没判断好就跪了！！！多么痛的领悟！！！
	3.头指针指向的地址在被释放之后，头指针还指向那个地址，并没有改变（不过这块内存已经可以被别的变量所使用了）
	4.在堆空间中，一个类型的内存，刚刚被释放掉之后，立马申请同样的类型的内存，还是会申请到那块刚刚被释放的内存
	


*/
/************************************************************************/


#include "stdafx.h"
#define DefPass "123456"

typedef struct AccoutInfo   //账号信息
{
	char Accout[21]; 
	char Password[21];
	//bool Flag;//true是老师，flase是学生 
}ACCOUT;
typedef struct StudentInfo  //学生信息
{
	ACCOUT a_AccoutInfo;
	int iStdentNo;
	char cName[20];
	char cSex[10];  // 男 ， 女
	char cClass[10];
	int bFlag; //0 '学生' 1 '老师'
}STUIFO;

typedef struct StuNodeInfo  //链表存储学生信息
{
	//char* node;
	STUIFO s_StudentInfo;
	struct StuNodeInfo* pNext;

}STUNODE;
STUNODE* StuHead=NULL;


ACCOUT Register();//注册账号
bool ModifyRegister(ACCOUT AccoutNo);//验证账号是否存在
bool ModifyLogin(ACCOUT login);//验证能否登录
void PrintStuInfo(STUIFO student);//打印学生信息
STUNODE* CreatNodeList();//创建链表头节点
bool InitStu();//初始化文件到链表中     有回档风险
STUNODE* ModifyStuInfo(ACCOUT* User);
void AddStuInfo(ACCOUT accout);
void DelStuInfo();
void AltStuInfo();
void QueStuInfo();
bool WriteStuInfo();//在文件中写入学生信息
bool WriteAccout(ACCOUT* accout);//更改过的数据写入文件并释放链表内存空间
int _tmain(int argc, _TCHAR* argv[])
{
	//初始化将数据载入到链表中
	StuHead=CreatNodeList();
	if (!StuHead)
	{
		printf("error\n");
		return 0;
	}
	if (!InitStu())
	{
		printf("error\n");
		return 0;
	}
	//登录账号
	STUNODE* Student;//=(STUIFO*)malloc(sizeof(STUIFO));
	ACCOUT* User=(ACCOUT*)malloc(sizeof(ACCOUT));
	printf("**********欢迎进入学生管理系统**********\n");
	while(1){
		printf("账号：（第一次登陆请输入“res”进行注册）\n");
		gets(User->Accout);
		//注册
		if (strcmp(User->Accout,"res")==NULL)
		{
			*User=Register();
			if (User->Accout!="0")
			{
				printf("恭喜注册成功！\n");
				break;
			}else
			{
				system("pause");
				return 0;
			}
		}
		//登录
		printf("请输入密码：\n");
		gets(User->Password);
		if(ModifyLogin(*User))
		{
			printf("*****************登录成功********************\n");
			break;
		}
		else
		{
			printf("账号或密码有误，请重新输入：\n");
			continue;
		}
	}
	
	//提取学生信息
	Student=ModifyStuInfo(User);
	if (!Student)
	{
		printf("用户数据不存在！\n");
		system("pause");
		return 0;
	}
	if (strcmp(Student->s_StudentInfo.a_AccoutInfo.Accout,"Administrator")==0)
	{
		Student->s_StudentInfo.bFlag=1;
	}
	//判断当前账户是学生还是老师
	if (Student->s_StudentInfo.bFlag==0)
	{
		printf("************你以学生账号登入**************\n");
		PrintStuInfo(Student->s_StudentInfo);
		system("pause");
		return 0;
	}
	else if(Student->s_StudentInfo.bFlag==1)
	{
		int OperaType=0;
		do 
		{
			printf("\n**************你以老师账号登入**************\n");
			printf("请选择对学生信息进行的操作（1.增加 2.删除 3.修改 4.查询 0.退出 ）\n");
			scanf("%d",&OperaType);
			switch(OperaType){
			case 0:
				break;
			case 1:
				ACCOUT DefAccout;
				strcpy(DefAccout.Accout,"0");
				strcpy(DefAccout.Password,"0");
				AddStuInfo(DefAccout);
				break;
			case 2:
				DelStuInfo();
				break;
			case 3:
				AltStuInfo();
				break;
			case 4:
				QueStuInfo();
				break;
			default:
				break;
			}
		} while (OperaType!=0);
		
	}
	free(User);
	User=NULL;
	//将修改过的数据写入文件中，并释放链表占用的内存空间
	bool flag=WriteStuInfo();
	if (!flag)
	{
		printf("更改信息入库失败！\n");
	}
	else if(flag)
	{
		printf("更改信息入库成功！\n");
	}
	else
	{
		printf("error!\n");
	}


	system("pause");
	return 0;
}
ACCOUT Register()
{
	int times=0;
	ACCOUT RegAccout;
	char cPassword_again[21];
	printf("**********注册学生管理系统**********\n");
	
	while (times<5)
	{
		printf("账号：（6~20个字符）");
		gets(RegAccout.Accout);
		printf("密码：（6~20个字符）");
		gets(RegAccout.Password);
		printf("再次确认密码：");
		gets(cPassword_again);

		//判断账号是否符合规则，不能与已有账号重复，位数大于6小于20
		if (strlen(RegAccout.Accout)<6 || strlen(RegAccout.Accout)>20)
		{
			printf("输入的账号长度不符合要求（6~20），请重新输入：\n");
			times++;
			continue;
		}
		if (!ModifyRegister(RegAccout))
		{
            //验证账号是否已经被注册
			times++;
			continue;

		}
		if (strlen(RegAccout.Password)<6 || strlen(RegAccout.Password)>20)
		{
			printf("输入的密码长度不符合要求（6~20），请重新输入：\n");
			times++;
			continue;
		}
		if (strcmp(RegAccout.Password,cPassword_again)!=NULL)
		{
			printf("两次输入的密码不一致，请重新输入密码：\n");
			times++;
			continue;
		}
		break;
		
	}
	if (times==5)
	{
		printf("你是不是傻，注册个账号输这么多遍！\n");
		strcpy(RegAccout.Accout,"0");
		return RegAccout;
	}

	FILE* file=fopen("AccoutInfo.txt","a");
	if (file==NULL)
	{
		printf("打开账户数据库失败！");
		fclose(file);
		strcpy(RegAccout.Accout,"0");
		return RegAccout;
	}
	
		if (fwrite(&RegAccout,sizeof(ACCOUT),1,file)>0)
		{
			fclose(file);
			//注册信息成功以后添加学生信息
			AddStuInfo(RegAccout);
			return RegAccout;
		}
		else
		{
			printf("写入注册信息失败！");
			fclose(file);
			strcpy(RegAccout.Accout,"0");
			return RegAccout;
		}

	
}

//功能：验证账号是够已经存在
bool ModifyRegister(ACCOUT AccoutNo)
{
	ACCOUT ModifyAccout;
	//char CoompareAccout[21];
	FILE* file=fopen("AccoutInfo.txt","r");
	if (file==NULL)
	{
		printf("打开账户数据库失败！");
		fclose(file);
		return false;
	}
	while(fread(&ModifyAccout,sizeof(ACCOUT),1,file)>0)
	{
		if (strcmp(AccoutNo.Accout,ModifyAccout.Accout)==0)
		{
			printf("改账号已被注册！\n");
			fclose(file);
			return false;
		}
	}
	
		fclose(file);
		return true;
}
bool ModifyLogin(ACCOUT login)
{
	ACCOUT ModifyUser;
	FILE* file=fopen("AccoutInfo.txt","r");
	if (file==NULL)
	{
		printf("打开账户数据库失败！");
		fclose(file);
		return false;
	}
	//fseek(file,0,SEEK_SET)；
	//fread()查找到文件结尾时返回0；
	while (fread(&ModifyUser,sizeof(ACCOUT),1,file)!=0)
	{
		if (strcmp(login.Accout,ModifyUser.Accout)==NULL)
		{
			if (strcmp(login.Password,ModifyUser.Password)==NULL)
			{
				fclose(file);
				return true;
			}
		}
		//fseek(file,sizeof(ACCOUT),SEEK_CUR);
	}
	fclose(file);
	return false;
}
void PrintStuInfo(STUIFO student)
{
	printf("学号：%d\n",student.iStdentNo);
	printf("姓名：%s\n",student.cName);
	printf("性别：%s\n",student.cSex);
	printf("班级：%s\n",student.cClass);
}
STUNODE* CreatNodeList()
{

	StuHead=(STUNODE*)malloc(sizeof(STUNODE));
	if (StuHead==NULL)
	{ 
		return false;
	}
	else
	{
		//头节点保存的值未初始化
		StuHead->pNext=NULL;
		if (StuHead->pNext==NULL)
		{
			printf("初始化头节点成功\n");
		}
		else
		{
			printf("初始化头节点失败\n");
		}
		return StuHead;
	}

}
bool InitStu()
{
	STUIFO* sStudent=(STUIFO*)malloc(sizeof(STUIFO));
	FILE* file=fopen("StuInfo.txt","r");
	if (file==NULL)
	{
		printf("打开账户数据库失败！");
		fclose(file);
		return false;
	}
	while(1)
	{
		if (fread(sStudent,sizeof(STUIFO),1,file)>0)
		{
			STUNODE* sNode=(STUNODE*)malloc(sizeof(STUNODE));
			sNode->s_StudentInfo=*sStudent;//传递的是数据
			sNode->pNext=NULL;
			if (StuHead->pNext==NULL)
			{
				StuHead->pNext=sNode;
			}
			else
			{
				sNode->pNext=StuHead->pNext;
				StuHead->pNext=sNode;
			}
		}
		else
		{
			break;
		}
	}
	fclose(file);
	free(sStudent);
	sStudent=NULL;
	return true;
	
}
STUNODE* ModifyStuInfo(ACCOUT* User)
{
	STUNODE* s_ModifyStuInfo;
	//bug：
	//定义了一个STUNODE*类型的指针s_ModifyStuInfo，指向一个申请到的STUNODE大小空间
	//！！！！！！！！！！接着又把这个指针指向了头节点！！！！！！！！！！！！！！！
	//在执行完函数之后释放这个指针指向的空间，释放的是！！！！指向的头节点的堆空间！！！！，而不是申请到的堆空间；
	//导致到后面再申请STUNODE*类型指针并分配STUNODE大小的堆空间的时候，还是申请到刚刚释放的堆空间
	//结果就是头指针与新申请到的指针指向同一块地址；
	/************************************************************************/
	/* 总结说明：
	1.释放指针空间，释放的是指针目前指向的空间，释放时一定要判断清楚，这个指针现在指向哪里！
	2.链表在初始化、增加节点时需要申请空间，其他时候只定义一个链接类型的指针就好！不要分配空间！分配完了还得释放，释放时没判断好就跪了！！！多么痛的领悟！！！
	3.头指针指向的地址在被释放之后，头指针还指向那个地址，并没有改变（不过这块内存已经可以被别的变量所使用了）
	4.在堆空间中，一个类型的内存，刚刚被释放掉之后，立马申请同样的类型的内存，还是会申请到那块刚刚被释放的内存
	*/
	/************************************************************************/
	s_ModifyStuInfo=StuHead->pNext;
	while (1)
	{
		if (s_ModifyStuInfo==NULL)
		{
			//说明链表中没有学生信息
			return s_ModifyStuInfo;
		}
		else if(strcmp(s_ModifyStuInfo->s_StudentInfo.a_AccoutInfo.Accout,User->Accout)==0 && strcmp(s_ModifyStuInfo->s_StudentInfo.a_AccoutInfo.Password,User->Password)==0 )
		{
			//该账号学生的信息是存在的
			return s_ModifyStuInfo;
		}
		else
		{
			//指向链表的下一个节点
			s_ModifyStuInfo=s_ModifyStuInfo->pNext;
		}

	}

}

void AddStuInfo(ACCOUT accout)
{
	STUNODE* AddStu=(STUNODE*)malloc(sizeof(STUNODE));
	printf("请输入学生学号：");
	scanf("%d",&(AddStu->s_StudentInfo.iStdentNo));
	getchar();
	printf("请输入学生姓名：");
	gets(AddStu->s_StudentInfo.cName);
	printf("请输入学生性别：");
	gets(AddStu->s_StudentInfo.cSex);
	//限定只能输入“男”或者“女”
	printf("请输入学生班级：");
	gets(AddStu->s_StudentInfo.cClass);
	AddStu->s_StudentInfo.bFlag=0;
	//创建学生信息时 学号为默认账号，默认密码123456
	if (strcmp(accout.Accout,"0")==0 && strcmp(accout.Password,"0")==0)
	{
		ACCOUT DefAccout;
		char StuNo[10];
		itoa(AddStu->s_StudentInfo.iStdentNo,StuNo,10);//Int型转换为字符串
		strcpy(DefAccout.Accout,AddStu->s_StudentInfo.cName);
		strcpy(DefAccout.Password,DefPass);
		strcpy(AddStu->s_StudentInfo.a_AccoutInfo.Accout,strcat(DefAccout.Accout,StuNo));
		strcpy(AddStu->s_StudentInfo.a_AccoutInfo.Password,DefAccout.Password);
		//增加学生信息的时候同时写入默认的账户
		if (!WriteAccout(&AddStu->s_StudentInfo.a_AccoutInfo))
		{
			printf("error!\n");
		}
	}
	else
	{
		strcpy(AddStu->s_StudentInfo.a_AccoutInfo.Accout,accout.Accout);
		strcpy(AddStu->s_StudentInfo.a_AccoutInfo.Password,accout.Password);
	}
	
	if (StuHead->pNext==NULL)
	{
		StuHead->pNext=AddStu;
		AddStu->pNext=NULL;
	}
	else
	{
		AddStu->pNext=StuHead->pNext;
		StuHead->pNext=AddStu;
	}
	
}
void DelStuInfo()
{
	int StuNo=0;
	STUNODE* DelStu;
	STUNODE* PreDel;
	PreDel=StuHead;
	DelStu=StuHead->pNext;
	printf("输入你要删除的学生的学号：");
	scanf("%d",&StuNo);
	while(1)
	{
		if (DelStu==NULL)
		{
			printf("系统中无该学生信息！\n");
			break;
		}
		else if (StuNo==DelStu->s_StudentInfo.iStdentNo)
		{
			PreDel->pNext=DelStu->pNext;
			printf("删除学生信息成功！\n");
			free(DelStu);
			DelStu=NULL;
			break;
		}
		else
		{
			PreDel=DelStu;
			DelStu=DelStu->pNext;
		}
	}
	
}
void AltStuInfo()
{
	int StuNo=0;
	STUNODE* AltStu;
	AltStu=StuHead->pNext;
	printf("请输入你要修改的学生的学号：");
	scanf("%d",&StuNo);
	while (1)
	{
		if (AltStu==NULL)
		{
			printf("系统中无任何学生信息！\n");
			break;
		}
		else if (AltStu->s_StudentInfo.iStdentNo==StuNo)
		{
			printf("你要修改的学生信息如下：\n");
			PrintStuInfo(AltStu->s_StudentInfo);
			int Operate=0;
			do 
			{
				printf("请输入你要修改的学生信息内容：0.退出修改 1.姓名 2.性别 3.班级 4.身份\n");
				scanf("%d",&Operate);
				getchar();
				switch(Operate)
				{
				case 0:
					break;
				case 1:
					printf("输入新的学生姓名：");
					gets(AltStu->s_StudentInfo.cName);
					break;
				case 2:
					printf("输入新的学生性别：");
					gets(AltStu->s_StudentInfo.cSex);
					break;
				case 3:
					printf("输入新的学生班级：");
					gets(AltStu->s_StudentInfo.cClass);
					break;
				case 4:
					printf("输入新的身份：学生：0 老师：1\n");
					scanf("%d",&AltStu->s_StudentInfo.bFlag);
					break;
				default:
					printf("输入有误，请重新输入：\n");
					break;
				}
			} while (Operate!=0);
			printf("修改过的学生信息如下：\n");
			PrintStuInfo(AltStu->s_StudentInfo);
			break;
		}
		else
		{
			AltStu=AltStu->pNext;
		}
	}
	
}
void QueStuInfo()
{
	
	STUNODE* s_ModifyStuInfo;
	STUIFO* qStu=(STUIFO*)malloc(sizeof(STUIFO));
	s_ModifyStuInfo=StuHead->pNext;
	printf("请输入你要查询的学生的姓名：");
	scanf("%s",qStu->cName);
	while (1)
	{
		if (s_ModifyStuInfo==NULL)
		{
			//说明链表中没有学生信息
			printf("你要查找的学生不存在！\n");
			return;
		}
		else if(strcmp(s_ModifyStuInfo->s_StudentInfo.cName,qStu->cName)==0)
		{
			//该账号学生的信息是存在的
			PrintStuInfo(s_ModifyStuInfo->s_StudentInfo);
			return;
		}
		else
		{
			//指向链表的下一个节点
			s_ModifyStuInfo=s_ModifyStuInfo->pNext;
		}

	}
}
bool WriteStuInfo()
{
	STUNODE* FreeStu;
	FreeStu=StuHead->pNext;
	FILE* file=fopen("StuInfo.txt","w+");
	if (file==NULL)
	{
		printf("连接数据库失败！\n");
		fclose(file);
		return false;
	}
	while (FreeStu!=NULL)
	{
		if(fwrite(&(FreeStu->s_StudentInfo),sizeof(STUIFO),1,file)>0)
		{

			FreeStu=FreeStu->pNext;
		}
		else
		{
			printf("写入数据失败！\n");
			break;
		}
	}
	while(StuHead!=NULL)
	{
		FreeStu=StuHead;
		StuHead=StuHead->pNext;
		free(FreeStu);
		FreeStu=NULL;
	}
	fclose(file);
	return true;
} 
bool WriteAccout(ACCOUT* accout)
{
	FILE* file=fopen("AccoutInfo.txt","a");
	if (file==NULL)
	{
		printf("写入账户信息失败！\n");
		fclose(file);
		return false;
	}
	else if (fwrite(accout,sizeof(ACCOUT),1,file)>0)
	{
		printf("账户信息写入成功！\n");
		fclose(file);
		return true;
	}
	else
	{
		printf("error！\n");
		return false;
	}
}