
// SmartStudentManger.cpp : �������̨Ӧ�ó������ڵ㡣
//
/************************************************************************/
/*ѧ������ϵͳ��
ע��->��½->ѧ������ѯ�Լ������Ϣ�ģ�
          ->��ʦ������ԱȨ�ޣ�����ɾ���ġ���

1.�����˺�ʱ��Ҫ��д���������Ϣ
2.������Ա�˻�����ʦ�˻���¼ʱ�������û���Ϣ����Ĭ���˻����� 
�˻���ѧ������+ѧ��
���룺123456

StuInfo.txt->����ѧ���������Ϣ         
AccoutInfo.txt->�����˺�������Ϣ

ͨ�������ȡ�ļ���Ϣ�����в�������ɾ�ĵ�ʱ��д���ļ��������η����ļ���Ӱ��ϵͳЧ��


2016.11.20
Q��ע���˺�ʱ��ͨ��fwrite()���ṹ��д���ļ�֮���ַ���ĩβΪ' '(�ո�)
�����˺�ͨ��gets()�����������ַ���ĩβ��'\0';
���µ�¼ʧ��
A:�ṹ���д���bool������Ϊ��ֵ���������ݴ��ڴ���

2016.11.27
bug��
	������һ��STUNODE*���͵�ָ��s_ModifyStuInfo��ָ��һ�����뵽��STUNODE��С�ռ�
	�������������������������ְ����ָ��ָ����ͷ�ڵ㣡����������������������������
	��ִ���꺯��֮���ͷ����ָ��ָ��Ŀռ䣬�ͷŵ��ǣ�������ָ���ͷ�ڵ�Ķѿռ䣡�����������������뵽�Ķѿռ䣻
	���µ�����������STUNODE*����ָ�벢����STUNODE��С�Ķѿռ��ʱ�򣬻������뵽�ո��ͷŵĶѿռ�
	�������ͷָ���������뵽��ָ��ָ��ͬһ���ַ��
	 �ܽ�˵����
	1.�ͷ�ָ��ռ䣬�ͷŵ���ָ��Ŀǰָ��Ŀռ䣬�ͷ�ʱһ��Ҫ�ж���������ָ������ָ�����
	2.�����ڳ�ʼ�������ӽڵ�ʱ��Ҫ����ռ䣬����ʱ��ֻ����һ���������͵�ָ��ͺã���Ҫ����ռ䣡�������˻����ͷţ��ͷ�ʱû�жϺþ͹��ˣ�������ôʹ�����򣡣���
	3.ͷָ��ָ��ĵ�ַ�ڱ��ͷ�֮��ͷָ�뻹ָ���Ǹ���ַ����û�иı䣨��������ڴ��Ѿ����Ա���ı�����ʹ���ˣ�
	4.�ڶѿռ��У�һ�����͵��ڴ棬�ոձ��ͷŵ�֮����������ͬ�������͵��ڴ棬���ǻ����뵽�ǿ�ոձ��ͷŵ��ڴ�
	


*/
/************************************************************************/


#include "stdafx.h"
#define DefPass "123456"

typedef struct AccoutInfo   //�˺���Ϣ
{
	char Accout[21]; 
	char Password[21];
	//bool Flag;//true����ʦ��flase��ѧ�� 
}ACCOUT;
typedef struct StudentInfo  //ѧ����Ϣ
{
	ACCOUT a_AccoutInfo;
	int iStdentNo;
	char cName[20];
	char cSex[10];  // �� �� Ů
	char cClass[10];
	int bFlag; //0 'ѧ��' 1 '��ʦ'
}STUIFO;

typedef struct StuNodeInfo  //����洢ѧ����Ϣ
{
	//char* node;
	STUIFO s_StudentInfo;
	struct StuNodeInfo* pNext;

}STUNODE;
STUNODE* StuHead=NULL;


ACCOUT Register();//ע���˺�
bool ModifyRegister(ACCOUT AccoutNo);//��֤�˺��Ƿ����
bool ModifyLogin(ACCOUT login);//��֤�ܷ��¼
void PrintStuInfo(STUIFO student);//��ӡѧ����Ϣ
STUNODE* CreatNodeList();//��������ͷ�ڵ�
bool InitStu();//��ʼ���ļ���������     �лص�����
STUNODE* ModifyStuInfo(ACCOUT* User);
void AddStuInfo(ACCOUT accout);
void DelStuInfo();
void AltStuInfo();
void QueStuInfo();
bool WriteStuInfo();//���ļ���д��ѧ����Ϣ
bool WriteAccout(ACCOUT* accout);//���Ĺ�������д���ļ����ͷ������ڴ�ռ�
int _tmain(int argc, _TCHAR* argv[])
{
	//��ʼ�����������뵽������
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
	//��¼�˺�
	STUNODE* Student;//=(STUIFO*)malloc(sizeof(STUIFO));
	ACCOUT* User=(ACCOUT*)malloc(sizeof(ACCOUT));
	printf("**********��ӭ����ѧ������ϵͳ**********\n");
	while(1){
		printf("�˺ţ�����һ�ε�½�����롰res������ע�ᣩ\n");
		gets(User->Accout);
		//ע��
		if (strcmp(User->Accout,"res")==NULL)
		{
			*User=Register();
			if (User->Accout!="0")
			{
				printf("��ϲע��ɹ���\n");
				break;
			}else
			{
				system("pause");
				return 0;
			}
		}
		//��¼
		printf("���������룺\n");
		gets(User->Password);
		if(ModifyLogin(*User))
		{
			printf("*****************��¼�ɹ�********************\n");
			break;
		}
		else
		{
			printf("�˺Ż������������������룺\n");
			continue;
		}
	}
	
	//��ȡѧ����Ϣ
	Student=ModifyStuInfo(User);
	if (!Student)
	{
		printf("�û����ݲ����ڣ�\n");
		system("pause");
		return 0;
	}
	if (strcmp(Student->s_StudentInfo.a_AccoutInfo.Accout,"Administrator")==0)
	{
		Student->s_StudentInfo.bFlag=1;
	}
	//�жϵ�ǰ�˻���ѧ��������ʦ
	if (Student->s_StudentInfo.bFlag==0)
	{
		printf("************����ѧ���˺ŵ���**************\n");
		PrintStuInfo(Student->s_StudentInfo);
		system("pause");
		return 0;
	}
	else if(Student->s_StudentInfo.bFlag==1)
	{
		int OperaType=0;
		do 
		{
			printf("\n**************������ʦ�˺ŵ���**************\n");
			printf("��ѡ���ѧ����Ϣ���еĲ�����1.���� 2.ɾ�� 3.�޸� 4.��ѯ 0.�˳� ��\n");
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
	//���޸Ĺ�������д���ļ��У����ͷ�����ռ�õ��ڴ�ռ�
	bool flag=WriteStuInfo();
	if (!flag)
	{
		printf("������Ϣ���ʧ�ܣ�\n");
	}
	else if(flag)
	{
		printf("������Ϣ���ɹ���\n");
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
	printf("**********ע��ѧ������ϵͳ**********\n");
	
	while (times<5)
	{
		printf("�˺ţ���6~20���ַ���");
		gets(RegAccout.Accout);
		printf("���룺��6~20���ַ���");
		gets(RegAccout.Password);
		printf("�ٴ�ȷ�����룺");
		gets(cPassword_again);

		//�ж��˺��Ƿ���Ϲ��򣬲����������˺��ظ���λ������6С��20
		if (strlen(RegAccout.Accout)<6 || strlen(RegAccout.Accout)>20)
		{
			printf("������˺ų��Ȳ�����Ҫ��6~20�������������룺\n");
			times++;
			continue;
		}
		if (!ModifyRegister(RegAccout))
		{
            //��֤�˺��Ƿ��Ѿ���ע��
			times++;
			continue;

		}
		if (strlen(RegAccout.Password)<6 || strlen(RegAccout.Password)>20)
		{
			printf("��������볤�Ȳ�����Ҫ��6~20�������������룺\n");
			times++;
			continue;
		}
		if (strcmp(RegAccout.Password,cPassword_again)!=NULL)
		{
			printf("������������벻һ�£��������������룺\n");
			times++;
			continue;
		}
		break;
		
	}
	if (times==5)
	{
		printf("���ǲ���ɵ��ע����˺�����ô��飡\n");
		strcpy(RegAccout.Accout,"0");
		return RegAccout;
	}

	FILE* file=fopen("AccoutInfo.txt","a");
	if (file==NULL)
	{
		printf("���˻����ݿ�ʧ�ܣ�");
		fclose(file);
		strcpy(RegAccout.Accout,"0");
		return RegAccout;
	}
	
		if (fwrite(&RegAccout,sizeof(ACCOUT),1,file)>0)
		{
			fclose(file);
			//ע����Ϣ�ɹ��Ժ����ѧ����Ϣ
			AddStuInfo(RegAccout);
			return RegAccout;
		}
		else
		{
			printf("д��ע����Ϣʧ�ܣ�");
			fclose(file);
			strcpy(RegAccout.Accout,"0");
			return RegAccout;
		}

	
}

//���ܣ���֤�˺��ǹ��Ѿ�����
bool ModifyRegister(ACCOUT AccoutNo)
{
	ACCOUT ModifyAccout;
	//char CoompareAccout[21];
	FILE* file=fopen("AccoutInfo.txt","r");
	if (file==NULL)
	{
		printf("���˻����ݿ�ʧ�ܣ�");
		fclose(file);
		return false;
	}
	while(fread(&ModifyAccout,sizeof(ACCOUT),1,file)>0)
	{
		if (strcmp(AccoutNo.Accout,ModifyAccout.Accout)==0)
		{
			printf("���˺��ѱ�ע�ᣡ\n");
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
		printf("���˻����ݿ�ʧ�ܣ�");
		fclose(file);
		return false;
	}
	//fseek(file,0,SEEK_SET)��
	//fread()���ҵ��ļ���βʱ����0��
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
	printf("ѧ�ţ�%d\n",student.iStdentNo);
	printf("������%s\n",student.cName);
	printf("�Ա�%s\n",student.cSex);
	printf("�༶��%s\n",student.cClass);
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
		//ͷ�ڵ㱣���ֵδ��ʼ��
		StuHead->pNext=NULL;
		if (StuHead->pNext==NULL)
		{
			printf("��ʼ��ͷ�ڵ�ɹ�\n");
		}
		else
		{
			printf("��ʼ��ͷ�ڵ�ʧ��\n");
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
		printf("���˻����ݿ�ʧ�ܣ�");
		fclose(file);
		return false;
	}
	while(1)
	{
		if (fread(sStudent,sizeof(STUIFO),1,file)>0)
		{
			STUNODE* sNode=(STUNODE*)malloc(sizeof(STUNODE));
			sNode->s_StudentInfo=*sStudent;//���ݵ�������
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
	//bug��
	//������һ��STUNODE*���͵�ָ��s_ModifyStuInfo��ָ��һ�����뵽��STUNODE��С�ռ�
	//�������������������������ְ����ָ��ָ����ͷ�ڵ㣡����������������������������
	//��ִ���꺯��֮���ͷ����ָ��ָ��Ŀռ䣬�ͷŵ��ǣ�������ָ���ͷ�ڵ�Ķѿռ䣡�����������������뵽�Ķѿռ䣻
	//���µ�����������STUNODE*����ָ�벢����STUNODE��С�Ķѿռ��ʱ�򣬻������뵽�ո��ͷŵĶѿռ�
	//�������ͷָ���������뵽��ָ��ָ��ͬһ���ַ��
	/************************************************************************/
	/* �ܽ�˵����
	1.�ͷ�ָ��ռ䣬�ͷŵ���ָ��Ŀǰָ��Ŀռ䣬�ͷ�ʱһ��Ҫ�ж���������ָ������ָ�����
	2.�����ڳ�ʼ�������ӽڵ�ʱ��Ҫ����ռ䣬����ʱ��ֻ����һ���������͵�ָ��ͺã���Ҫ����ռ䣡�������˻����ͷţ��ͷ�ʱû�жϺþ͹��ˣ�������ôʹ�����򣡣���
	3.ͷָ��ָ��ĵ�ַ�ڱ��ͷ�֮��ͷָ�뻹ָ���Ǹ���ַ����û�иı䣨��������ڴ��Ѿ����Ա���ı�����ʹ���ˣ�
	4.�ڶѿռ��У�һ�����͵��ڴ棬�ոձ��ͷŵ�֮����������ͬ�������͵��ڴ棬���ǻ����뵽�ǿ�ոձ��ͷŵ��ڴ�
	*/
	/************************************************************************/
	s_ModifyStuInfo=StuHead->pNext;
	while (1)
	{
		if (s_ModifyStuInfo==NULL)
		{
			//˵��������û��ѧ����Ϣ
			return s_ModifyStuInfo;
		}
		else if(strcmp(s_ModifyStuInfo->s_StudentInfo.a_AccoutInfo.Accout,User->Accout)==0 && strcmp(s_ModifyStuInfo->s_StudentInfo.a_AccoutInfo.Password,User->Password)==0 )
		{
			//���˺�ѧ������Ϣ�Ǵ��ڵ�
			return s_ModifyStuInfo;
		}
		else
		{
			//ָ���������һ���ڵ�
			s_ModifyStuInfo=s_ModifyStuInfo->pNext;
		}

	}

}

void AddStuInfo(ACCOUT accout)
{
	STUNODE* AddStu=(STUNODE*)malloc(sizeof(STUNODE));
	printf("������ѧ��ѧ�ţ�");
	scanf("%d",&(AddStu->s_StudentInfo.iStdentNo));
	getchar();
	printf("������ѧ��������");
	gets(AddStu->s_StudentInfo.cName);
	printf("������ѧ���Ա�");
	gets(AddStu->s_StudentInfo.cSex);
	//�޶�ֻ�����롰�С����ߡ�Ů��
	printf("������ѧ���༶��");
	gets(AddStu->s_StudentInfo.cClass);
	AddStu->s_StudentInfo.bFlag=0;
	//����ѧ����Ϣʱ ѧ��ΪĬ���˺ţ�Ĭ������123456
	if (strcmp(accout.Accout,"0")==0 && strcmp(accout.Password,"0")==0)
	{
		ACCOUT DefAccout;
		char StuNo[10];
		itoa(AddStu->s_StudentInfo.iStdentNo,StuNo,10);//Int��ת��Ϊ�ַ���
		strcpy(DefAccout.Accout,AddStu->s_StudentInfo.cName);
		strcpy(DefAccout.Password,DefPass);
		strcpy(AddStu->s_StudentInfo.a_AccoutInfo.Accout,strcat(DefAccout.Accout,StuNo));
		strcpy(AddStu->s_StudentInfo.a_AccoutInfo.Password,DefAccout.Password);
		//����ѧ����Ϣ��ʱ��ͬʱд��Ĭ�ϵ��˻�
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
	printf("������Ҫɾ����ѧ����ѧ�ţ�");
	scanf("%d",&StuNo);
	while(1)
	{
		if (DelStu==NULL)
		{
			printf("ϵͳ���޸�ѧ����Ϣ��\n");
			break;
		}
		else if (StuNo==DelStu->s_StudentInfo.iStdentNo)
		{
			PreDel->pNext=DelStu->pNext;
			printf("ɾ��ѧ����Ϣ�ɹ���\n");
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
	printf("��������Ҫ�޸ĵ�ѧ����ѧ�ţ�");
	scanf("%d",&StuNo);
	while (1)
	{
		if (AltStu==NULL)
		{
			printf("ϵͳ�����κ�ѧ����Ϣ��\n");
			break;
		}
		else if (AltStu->s_StudentInfo.iStdentNo==StuNo)
		{
			printf("��Ҫ�޸ĵ�ѧ����Ϣ���£�\n");
			PrintStuInfo(AltStu->s_StudentInfo);
			int Operate=0;
			do 
			{
				printf("��������Ҫ�޸ĵ�ѧ����Ϣ���ݣ�0.�˳��޸� 1.���� 2.�Ա� 3.�༶ 4.���\n");
				scanf("%d",&Operate);
				getchar();
				switch(Operate)
				{
				case 0:
					break;
				case 1:
					printf("�����µ�ѧ��������");
					gets(AltStu->s_StudentInfo.cName);
					break;
				case 2:
					printf("�����µ�ѧ���Ա�");
					gets(AltStu->s_StudentInfo.cSex);
					break;
				case 3:
					printf("�����µ�ѧ���༶��");
					gets(AltStu->s_StudentInfo.cClass);
					break;
				case 4:
					printf("�����µ���ݣ�ѧ����0 ��ʦ��1\n");
					scanf("%d",&AltStu->s_StudentInfo.bFlag);
					break;
				default:
					printf("�����������������룺\n");
					break;
				}
			} while (Operate!=0);
			printf("�޸Ĺ���ѧ����Ϣ���£�\n");
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
	printf("��������Ҫ��ѯ��ѧ����������");
	scanf("%s",qStu->cName);
	while (1)
	{
		if (s_ModifyStuInfo==NULL)
		{
			//˵��������û��ѧ����Ϣ
			printf("��Ҫ���ҵ�ѧ�������ڣ�\n");
			return;
		}
		else if(strcmp(s_ModifyStuInfo->s_StudentInfo.cName,qStu->cName)==0)
		{
			//���˺�ѧ������Ϣ�Ǵ��ڵ�
			PrintStuInfo(s_ModifyStuInfo->s_StudentInfo);
			return;
		}
		else
		{
			//ָ���������һ���ڵ�
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
		printf("�������ݿ�ʧ�ܣ�\n");
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
			printf("д������ʧ�ܣ�\n");
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
		printf("д���˻���Ϣʧ�ܣ�\n");
		fclose(file);
		return false;
	}
	else if (fwrite(accout,sizeof(ACCOUT),1,file)>0)
	{
		printf("�˻���Ϣд��ɹ���\n");
		fclose(file);
		return true;
	}
	else
	{
		printf("error��\n");
		return false;
	}
}