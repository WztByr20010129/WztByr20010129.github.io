#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define DATASTART 16384
#define DATAEND 32768 
/*С������:�Ĵ���5,�Ĵ���6ָ����ڴ���������,���ݱ��浽�Ĵ���5��,��ô�ڴ��������е����費��Ҫ�޸�???*/ 
typedef struct axptr{
	int data;//��������� 
	short ptr;//ָ����ڴ������ݴ�ֵ 
	int memptr;//ָ���ڴ濪ʼ���ݵ�ָ�� 
} AX;
//���ļ��ж�ȡָ�� 
int readDIR(FILE *fp,char Memory[],int size); 
//����ָ�� 
int divideInstruct(char buffer[],int size,char dir[]); 
//������תʮ���� 
long long getTenth(char binary[],int size); 
//ȡ�� 
int getNegation(char binary[],int size);
//ʮ����ת������ 
int getBinary(int n);
//��ӡ������ڴ� 
void printDirect(char dir[][35],int size);
//���ַ����н�ȡ����Ҫ�Ĳ��� 
void subStringByPos(char buffer[],char *p,int start,int end,int pStart); 
//����ָ��Ĺ��� 
int anlaysis(char dir[]);
//�жϵ����ǵ�ַ�Ĵ����������ݼĴ���; 
int isPtrOrData(int n);
//�Ѷ����������ڴ��� ,���Ӧ�÷������յ��ڴ��ַ 
int PutInMemory(char Memory[],char beput[],int dataleft);
//����dest��ʲô���� 
void* getDest(int axdest,int *dataAx[],AX* axptrs[],int number);
//����from��ʲô���� 
void* getFrom(int axdest,int *dataAx[],AX* axptrs[],int number); 
//ÿ�δ�ӡ���е���Ϣ
void printInfo(int *dataAx[],AX* axptrs[],int ir,int flag,int ip);
//��װһ������,��������ѵ�ַ�Ĵ����е���д���ڴ�
int writeAs(char Memory[],short data,int dataleft,char ModifyShort[]); 
//��װһ������,���������ӡ������
void printDataArea(char Memory[],int start); 
//��װһ������,�۲��ַ���ڴ�ı仯���
void testAx(AX *axptrs[]); 
void d_10(char dir[],int *i,int *aptr,int *ip,short numbers[]);//d_10
void jump_f(int *i,int *aptr,int *ip,int dir_num,int jump_num);//jump_f
//ѡ���Ӧ�Ĵ�����������---->��CPU�ĺ���,����ֵ��dataleft,�������������һ����ʼ�� 
int switchMethods(int *dataAx[],AX* axptrs[],int d,char ModifyShort[],char dir[],int dataleft);  
char Memory[35*1024];
int flag; 
//��������ʼ 
int main(void){
	/*��һ�׶�����:˳����ȡָ��+��ȡָ
	�����������ָ���*/ 
	FILE *fp=NULL;
	char buffer[512][35];memset(Memory,0,sizeof(Memory));memset(buffer,0,sizeof(buffer));
	
	int dirsize=readDIR(fp,Memory,35*1024);	//��ȡ�ļ� 
	
	char dir[dirsize][33];short numbers[dirsize]={0}; //����ָ������ //���������������� 	
	memset(dir,0,sizeof(dir));
 
	int i=0;//��ʱ���� 
	int j=0;//��ʱ���� 
	
	/*��һ�׶ν���*/ 
	
	/*�ڶ��׶�����:������������ָ֤�������ִ��*/
	int ip=0;flag=0;int ir=0;//ָ��Ĵ���
	int ax1=0;int ax2=0;int ax3=0;int ax4=0;//���ݼĴ��� 
	AX ax5;ax5.data=0;ax5.ptr=NULL;AX ax6;ax6.data=0;ax6.ptr=NULL;AX ax7;ax7.data=0;ax7.ptr=NULL;AX ax8;ax8.data=0;ax8.ptr=NULL;//��ַ�Ĵ���
	ax5.memptr=-1;ax6.memptr=-1;ax7.memptr=-1;ax8.memptr=-1;//�ڴ��ַ��ָ��λ��,��ʼֵ����û���κε�ָ������,�����߼��͵�ַ�Ĵ����Ƿ�ָ�����ڴ��й� 
	int* dataAx[4]={&ax1,&ax2,&ax3,&ax4};AX* axptrs[4]={&ax5,&ax6,&ax7,&ax8};//��������ָ������,����֮��ֱ�ӸĶ����� 
	char ModifyShort[17];memset(ModifyShort,'0',sizeof(ModifyShort));//ģ���ڴ��е�������,С�� ��Ϊ�������������и������� 
	int dataleft,dataright;dataleft=DATASTART;dataright=dataleft+16;
	int direction,aptr;//���մ��ص�ָ����Ϣ	//printf("dirsize=%d\n",dirsize);
	
	/*�ڶ��׶�:�������岿�ֽ���*/	 
	
	/*�����׶�:�����߼�����ʼ*/ 
	
	for(aptr=0;i<=dirsize;aptr++,j++){
		buffer[i][j]=Memory[aptr];
		if(Memory[aptr]=='\n'){
			numbers[i]=divideInstruct(buffer[i],dirsize,dir[i]);//���ﷵ��һ��������(ָ��ĺ�16λ,��numbers[i]��,dir[i]����ָ��ǰ16λ,buffer[i]��ȫ��  
			sprintf(ModifyShort,"%d",numbers[i]);//ģ��short����,��������ָ��̶�short���͵�λ��,Ȼ�����16Ϊ��λ //printf("%s\n",ModifyShort)
			direction=anlaysis(dir[i]);//����ָ������һ��printf("direction:%d\n",direction);
//			printf("direction:%d,aptr=%d\n",direction,aptr);
			ir=getTenth(dir[i],16);//��irתΪ10���ƵĽ��,�����ir������i֮ǰ��ȡ,����������� 
//			printf("��ǰָ���irֵ:%d",ir);printf("ִ�еڼ���ָ��:%d,ָ��Ϊ:%s\n",i,dir[i]);
			if(direction==10){//��ת��ʱ��ԼĴ����ڵ���û��Ӱ�� 
				d_10(dir[i],&i,&aptr,&ip,numbers); 
			}
			if(direction!=10){//����ת��ʱ�� 
//				printf("aptr:normal:%d\n",aptr);
				dataleft=switchMethods(dataAx,axptrs,direction,ModifyShort,dir[i],dataleft);//����һ��dataleftֵprintf("flag=%d\n",flag);
				ip+=4;//ip������
				i++;//�����Ӱ��numbers[i]��ֵ,����i++��,û�п�ѡ����ֵ,��numbers[i]�е�ֵ����ʼ��Ϊ0,�ʱ�Ȼ������,numbers[i]��Ϊ0  
//				printf("dataleft=%d,ip=%d,direction=%d\n",dataleft,ip,direction); 	
			}
			j=-1;
//			printf("��ǰ����ipֵ:%d\n",ip); 
			//����:��ӡһ�����dataInfo 
			printInfo(dataAx,axptrs,ir,flag,ip);
			if(dataleft==-1){
				break;
			}
//			testAx(axptrs);
		}
	}
	
	/*�����׶�:���Ĵ����ִ������*/
	
	/*���Ľ׶�:��ӡ�ڴ�κʹ���ο�ʼ*/ 
	
	printDirect(buffer,128);//��ӡ������ڴ� 
	printDataArea(Memory,DATASTART);//��ӡ���ݶ��ڴ�
	
	/*���Ľ׶ν���*/ 
	return 0;
}
void jump_f(int *i,int *aptr,int *ip,int dir_num,int jump_num){
	*i=*i+(dir_num);//i=14; 
	if(*i!=0){
		*aptr=33*(*i)-1;//��ѧ���� 
	}
	else if(*i==0){
		*aptr=0;	
	}
	*ip+=jump_num;
}
void d_10(char dir[],int *i,int *aptr,int *ip,short numbers[]){
	char jump[5]={'\0'};
	subStringByPos(dir,jump,12,16,0);
	int jump_flag=getTenth(jump,5);//����ô������ת�� 
	int jump_num=numbers[*i];//��ת�Ĳ���,������ת-28,���������ָ���Լ��Ѿ�������,��һ��ִ�е���-28+4=-24��λ�õ�ָ�� 
	int dir_num=jump_num/4;//printf("numbers[i]=%d,i=%d\njump_num:%d\ndirnum:%d\n",numbers[i],i,jump_num,dir_num);//��ӡ
	if(jump_flag==0){//��������ת 
		jump_f(i,aptr,ip,dir_num,jump_num);  
//		printf("��������ת:��ת��:i:%d,ip:%d,*aptr:%d\n",i,ip,*aptr);//��������ת 
	}
	if(jump_flag==1&&flag==0){//flagΪ0��ת 
		jump_f(i,aptr,ip,dir_num,jump_num);  			
	}
	else if(jump_flag==1&&flag!=0){
		*ip+=4;
		*i=*i+1;
	}
	if(jump_flag==2&&flag==1){//flagΪ1��ת 
		jump_f(i,aptr,ip,dir_num,jump_num); 
		//printf("i=%d,flagΪ1��ת,Memory:%c\n",i,Memory[*aptr]);//�������֪��Memory[*aptr]��ʲô��?��һ�����з� 
	}
	else if(jump_flag==2&&flag!=1){
		*ip+=4;
		*i=*i+1;
	} 
	if(jump_flag==3&&flag==-1){//flagΪ-1��ת 
		jump_f(i,aptr,ip,dir_num,jump_num); 	
	}
	else if(jump_flag==3&&flag!=-1){
		*ip+=4;
		*i=*i+1;
	}  		
}
void testAx(AX *axptrs[]){//�۲��ַ�Ĵ����ı仯��� 
	//��ֵ���� 
	printf("\n");
	for(int i=0;i<4;i++){
		printf("��%d��,�ڴ��ַ:%d,ָ���ֵ:%d\t",i,axptrs[i]->memptr,axptrs[i]->ptr);
	}
	printf("\n");
}
void subStringByPos(char buffer[],char p[],int start,int end,int pStart){//����:��ȡ�ַ�����ǰnλ���������д洢,����ҿ� 
	for(int i=start;i<end;i++){
		p[pStart++]=buffer[i];
	}
	p[pStart]='\0';//�����ַ����Ľ����� 
}
int isPtrOrData(int n){
	if(n>0&&n<=4){
		return 0;//���ݼĴ��� 
	}else if(n>=5&&n<8){
		return 1;//��ַ�Ĵ��� 
	}else if(n==0){
		return -1;//������ 
	}
	printf("isptrOrdata:Error ԭ������switch_method�� �޷�����ڴ��ַ,n=%d\n",n); 
	return -2; 
}
void *getFrom(int axfrom,int *dataAx[],AX* axptrs[],int number){
	int ffrom=isPtrOrData(axfrom);
	if(ffrom==0){
		return dataAx[axfrom-1];
	}else if(ffrom==1){
		return axptrs[axfrom-5];
	}else if(ffrom==-1){
		return &number;
	}
}
void* getDest(int axdest,int *dataAx[],AX* axptrs[],int number){
	int fdest=isPtrOrData(axdest);
	if(fdest==0){
		return dataAx[axdest-1];
	}else if(fdest==1){
		return axptrs[axdest-5];
	}else if(fdest==-1){
		return &number;
	}
}
int switchMethods(int *dataAx[],AX* axptrs[],int d,char ModifyShort[],char dir[],int dataleft){//ȥ���� 
	char dest[5]={'\0'};char from[5]={'\0'}; 
	subStringByPos(dir,dest,8,12,0);subStringByPos(dir,from,12,16,0);//�������:1.�������ƶ� 2.�ƶ�������
	int axdest=getTenth(dest,5);int axfrom=getTenth(from,5);int number=atoi(ModifyShort);//�ɵ�ַ��ȡindex��ֵ,��ת��Ϊint�Ķ�ת��Ϊint 
	int fdest=isPtrOrData(axdest);int ffrom=isPtrOrData(axfrom);//����fdest,ffrom,flag 
//	printf("axdest=%d,axfrom=%d,fdest=%d,ffrom=%d\n",axdest,axfrom,fdest,ffrom);
	void *dest_final=getDest(axdest,dataAx,axptrs,number);//��ȡdest����һ����ַ�Ĵ���,��һ�����ݼĴ��� 
	void *from_final=getFrom(axfrom,dataAx,axptrs,number);//��ȡfrom����һ����ַ�Ĵ���,��һ�����ݼĴ��� 	
	//���Ͻ���˼Ĵ��������� 
	int curPtr=dataleft;//���ص�����,���������ڴ沿�ֵ��׷���ʲô������ 
	if(d==0){
		curPtr=-1;
	}
	else if(d==1){//����
//		printf("ֵ:%d\n",*(int *)from_final);
		if(ffrom==-1){//���������������ݼĴ��� 
			if(fdest==0){
//				*(dataAx[axdest-1])=number; 
				*(int *)dest_final=*(int *)from_final;
			}else if(fdest==1){
				((AX *)dest_final)->data=*(int *)from_final;
//				axptrs[axdest-5]->data=number;//�ṹ��ָ��
//				printf("��ַ:number:%d\n",number);	
			}
		}else{//��ַ�Ĵ��������ݼĴ����Ļ���ת�� 
			if(fdest==0&&ffrom==0){//���ݼĴ��� 
//				*dataAx[axdest-1]=*dataAx[axfrom-1];
				*(int *)dest_final=*(int *)from_final;
			}
			else if(fdest==0&&ffrom==1){//��ַ�Ĵ����е�ֵ�������ݼĴ����� 
//				short my_data=axptrs[axfrom-5]->ptr;
//				*dataAx[axdest-1]=(int)my_data;
				short my_data=((AX *)from_final)->ptr;
				*(int *)dest_final=(int)my_data;
//				printf("������data=%d,sizeof(%d)\n",*dataAx[axdest-1],sizeof(*dataAx[axdest-1]));
			}
			else if(fdest==1&&ffrom==1){ 
				((AX *)dest_final)->ptr=((AX *)from_final)->ptr;//ָ���ֵ 
				((AX *)dest_final)->memptr=((AX *)from_final)->memptr;//ָ��ĵ�ַҲ�ı� 
//				axptrs[axdest-5]->ptr=axptrs[axfrom-5]->ptr;
//				printf("axdest=%d\n",axptrs[axdest-5]->ptr);
			}else if(fdest==1&&ffrom==0){//���ݼĴ����е�ֵ�����ַ�Ĵ����� ,��ַ�Ĵ����п�����������!,���ʱ��Ͳ���д���ڴ���,�����޸��ڴ� 
//				int data=*dataAx[axfrom-1];	//�������е�ֵû�иı�
//				int bin=getBinary(data);sprintf(ModifyShort,"%d",bin);//dataҪ��ŵ��ڴ���
//				curPtr=PutInMemory(Memory,ModifyShort);//��ȡ���ص��ڴ�ָ��,��䵽�ڴ���
//				axptrs[axdest-5]->ptr=data;//��ַ�Ĵ�����ȡֵ
				int data=*(int *)from_final;
				((AX *)dest_final)->ptr=data;
//				printf("������ת����ַ��,data:%d\n",data);
				if(((AX *)dest_final)->memptr==-1){//�ж��ڴ��е�ֵ����ӻ����޸� 
					curPtr=writeAs(Memory,data,dataleft,ModifyShort);//curPtr���ڴ��ַ����һ����ʼλ
					((AX *)dest_final)->memptr=curPtr-16;//�ڴ���ָ���ֵ�Ŀ�ͷ�����Ҳ� 	
				}else {
					writeAs(Memory,data,((AX *)dest_final)->memptr,ModifyShort);
//					curPtr=curPtr-16;
				}			
//				printf("data=%d,bin=%d,ModifyShort:%s\n",data,bin,ModifyShort);
			}
		}
	}
	else if(d==2){
		if(ffrom==-1){//������ 
			if(fdest==0){ 
				*(int *)dest_final+=number;
			}
			else if(fdest==1){
				((AX *)dest_final)->data+=number;//data�����������,�����������ĺ���������� 
				short my_data=((AX *)dest_final)->data; 
				writeAs(Memory,my_data,((AX *)dest_final)->memptr,ModifyShort);//��д��ַ����ָ����ڴ������ֵ 
			}
		} 
		if(fdest==0&&ffrom==0){//������ 
			*(int *)dest_final=*(int *)dest_final+*(int *)from_final;
		}
		if(fdest==1&&ffrom==1){//��ַ�� 
			short data=((AX *)from_final)->ptr;
			((AX *)dest_final)->ptr+=data;
			//�޸ĵ�ַ����ָ���ڴ����,��������� 
			writeAs(Memory,data,((AX *)dest_final)->memptr,ModifyShort);//��д��ַ����ָ����ڴ������ֵ
		}
		if(fdest==0&&ffrom==1){
			*(int *)dest_final+=((AX *)from_final)->ptr;
		}
		if(fdest==1&&ffrom==0){
			((AX *)dest_final)->ptr+=*(int *)from_final;
			//�޸ĵ�ַ����ָ���ڴ����,��������� 
			writeAs(Memory,((AX *)dest_final)->ptr,((AX *)dest_final)->memptr,ModifyShort);//��д��ַ����ָ����ڴ������ֵ
		}
	}
	else if(d==3){
		if(ffrom==-1){//������ 
			if(fdest==0){ 
				*(int *)dest_final-=number;
			}
			else if(fdest==1){
				((AX *)dest_final)->data-=number;//data�����������,�����������ĺ���������� 
				short my_data=((AX *)dest_final)->data; 
				writeAs(Memory,my_data,((AX *)dest_final)->memptr,ModifyShort);//��д��ַ����ָ����ڴ������ֵ 
			}
		} 
		if(fdest==0&&ffrom==0){//������ 
			*(int *)dest_final=*(int *)dest_final-*(int *)from_final;
		}
		if(fdest==1&&ffrom==1){//��ַ�� 
			short data=((AX *)from_final)->ptr;
			((AX *)dest_final)->ptr-=data;
			//�޸ĵ�ַ����ָ���ڴ����,��������� 
			writeAs(Memory,data,((AX *)dest_final)->memptr,ModifyShort);//��д��ַ����ָ����ڴ������ֵ
		}
		if(fdest==0&&ffrom==1){
			*(int *)dest_final-=((AX *)from_final)->ptr;
		}
		if(fdest==1&&ffrom==0){
			((AX *)dest_final)->ptr-=*(int *)from_final;
			//�޸ĵ�ַ����ָ���ڴ����,��������� 
			writeAs(Memory,((AX *)dest_final)->ptr,((AX *)dest_final)->memptr,ModifyShort);//��д��ַ����ָ����ڴ������ֵ
		}
	}
	else if(d==4){
			if(ffrom==-1){//������ 
			if(fdest==0){ 
				*(int *)dest_final*=number;
			}
			else if(fdest==1){
				((AX *)dest_final)->data*=number;//data�����������,�����������ĺ���������� 
				short my_data=((AX *)dest_final)->data; 
				writeAs(Memory,my_data,((AX *)dest_final)->memptr,ModifyShort);//��д��ַ����ָ����ڴ������ֵ 
			}
		} 
		if(fdest==0&&ffrom==0){//������ 
			*(int *)dest_final=*(int *)dest_final*(*(int *)from_final);
		}
		if(fdest==1&&ffrom==1){//��ַ�� 
			short data=((AX *)from_final)->ptr;
			((AX *)dest_final)->ptr*=data;
			//�޸ĵ�ַ����ָ���ڴ����,��������� 
			writeAs(Memory,data,((AX *)dest_final)->memptr,ModifyShort);//��д��ַ����ָ����ڴ������ֵ
		}
		if(fdest==0&&ffrom==1){
			*(int *)dest_final*=((AX *)from_final)->ptr;
		}
		if(fdest==1&&ffrom==0){
			((AX *)dest_final)->ptr*=*(int *)from_final;
			//�޸ĵ�ַ����ָ���ڴ����,��������� 
			writeAs(Memory,((AX *)dest_final)->ptr,((AX *)dest_final)->memptr,ModifyShort);//��д��ַ����ָ����ڴ������ֵ
		}
	}
	else if(d==5){
			if(ffrom==-1){//������ 
			if(fdest==0){ 
				*(int *)dest_final/=number;
			}
			else if(fdest==1){
				((AX *)dest_final)->data/=number;//data�����������,�����������ĺ���������� 
				short my_data=((AX *)dest_final)->data; 
				writeAs(Memory,my_data,((AX *)dest_final)->memptr,ModifyShort);//��д��ַ����ָ����ڴ������ֵ 
			}
		} 
		if(fdest==0&&ffrom==0){//������ 
			*(int *)dest_final=*(int *)dest_final/(*(int *)from_final);
		}
		if(fdest==1&&ffrom==1){//��ַ�� 
			short data=((AX *)from_final)->ptr;
			((AX *)dest_final)->ptr/=data;
			//�޸ĵ�ַ����ָ���ڴ����,��������� 
			writeAs(Memory,data,((AX *)dest_final)->memptr,ModifyShort);//��д��ַ����ָ����ڴ������ֵ
		}
		if(fdest==0&&ffrom==1){
			*(int *)dest_final/=((AX *)from_final)->ptr;
		}
		if(fdest==1&&ffrom==0){
			((AX *)dest_final)->ptr/=*(int *)from_final;
			//�޸ĵ�ַ����ָ���ڴ����,��������� 
			writeAs(Memory,((AX *)dest_final)->ptr,((AX *)dest_final)->memptr,ModifyShort);//��д��ַ����ָ����ڴ������ֵ
		}
	}
	else if(d==6){//�߼��� 
		if(ffrom==-1){
			int fromdata=*(int *)from_final;
			if(fdest==0){
				int destdata=*(int *)dest_final;
				*(int *)dest_final=fromdata&&destdata;
			}
			if(fdest==1){//�޸��ڴ�
				int destdata=(int)((AX*)dest_final)->ptr;
				((AX*)dest_final)->ptr=fromdata&&destdata;//�޸�ֵ 
				writeAs(Memory,fromdata&&destdata,((AX*)dest_final)->memptr,ModifyShort);//�޸��ڴ�			
			}	
		}
		if(ffrom==0&&fdest==0){
			*(int *)dest_final=(*(int *)dest_final)&&(*(int *)from_final);//�޸�ֵ 
		}
		if(ffrom==1&&fdest==0){
			*(int *)dest_final=((AX *)from_final)->ptr&&(*(int *)dest_final);//�޸�ֵ 
		}
		if(ffrom==0&&fdest==1){
			int datafrom=*(int *)from_final;
			int datadest=((AX*)dest_final)->ptr;
			((AX*)dest_final)->ptr=datafrom&&datadest;
			writeAs(Memory,((AX*)dest_final)->ptr,((AX*)dest_final)->memptr,ModifyShort);
		}
		if(ffrom==1&&fdest==1){
			int data1=((AX*)dest_final)->ptr;
			int data2=((AX*)from_final)->ptr;
			int dataflag=data1&&data2;
			((AX*)dest_final)->ptr=dataflag;
			writeAs(Memory,dataflag,((AX*)dest_final)->memptr,ModifyShort);//�޸��ڴ�
		}
	}
	else if(d==7){//�߼��� 
		if(ffrom==-1){
			int fromdata=*(int *)from_final;
			if(fdest==0){
				int destdata=*(int *)dest_final;
				*(int *)dest_final=fromdata||destdata;
			}
			if(fdest==1){//�޸��ڴ�
				int destdata=(int)((AX*)dest_final)->ptr;
				((AX*)dest_final)->ptr=fromdata||destdata;//�޸�ֵ 
				writeAs(Memory,fromdata||destdata,((AX*)dest_final)->memptr,ModifyShort);//�޸��ڴ�			
			}	
		}
		if(ffrom==0&&fdest==0){
			*(int *)dest_final=(*(int *)dest_final)||(*(int *)from_final);//�޸�ֵ 
		}
		if(ffrom==1&&fdest==0){
			*(int *)dest_final=((AX *)from_final)->ptr||(*(int *)dest_final);//�޸�ֵ 
		}
		if(ffrom==0&&fdest==1){
			int datafrom=*(int *)from_final;
			int datadest=((AX*)dest_final)->ptr;
			((AX*)dest_final)->ptr=datafrom&&datadest;
			writeAs(Memory,((AX*)dest_final)->ptr,((AX*)dest_final)->memptr,ModifyShort);//�޸��ڴ�	
		}
		if(ffrom==1&&fdest==1){
			int data1=((AX*)dest_final)->ptr;
			int data2=((AX*)from_final)->ptr;
			int dataflag=data1||data2;
			((AX*)dest_final)->ptr=dataflag;
			writeAs(Memory,dataflag,((AX*)dest_final)->memptr,ModifyShort);//�޸��ڴ�
		} 
	}
	else if(d==8){//�߼��� 
		 if(ffrom==-1){//��ĳ�����ݼĴ����ڵ������߼������� 
		 	*(int *)dest_final=!(*(int *)dest_final); 
		 }else if(fdest==-1){//��ĳ����ַ�Ĵ����ڵ������߼������� 
		 	short data=!((AX *)from_final)->ptr;
		 	((AX *)from_final)->ptr=data;
		 	writeAs(Memory,data,((AX *)from_final)->memptr,ModifyShort);//�޸��ڴ�	
		 }
	}
	else if(d==9){//�Ƚϲ���
		 if(ffrom==-1){//������ 
			int curnum=*(int *)from_final;
		 	if(fdest==0){//���ݼĴ��� 
		 		int ddata=*(int *)dest_final;
				if(ddata<curnum){
					flag=-1;
				}else if(ddata==curnum){
					flag=0;
				}else if(ddata>curnum){
					flag=1;
				}	
			}else if(fdest==1){//��ַ�Ĵ��� 
				int ddata=((AX*)dest_final)->ptr;
				if(ddata<curnum){
					flag=-1;
				}else if(ddata==curnum){
					flag=0;
				}else if(ddata>curnum){
					flag=1;
				}	 
			}	
		 }else{
		 	if(ffrom==0&&fdest==0){
		 		int ddata=*(int *)dest_final;
		 		int curnum=*(int *)from_final;
				if(ddata<curnum){
					flag=-1;
				}else if(ddata==curnum){
					flag=0;
				}else if(ddata>curnum){
					flag=1;
				}	
			}
			if(ffrom==1&&fdest==0){
				int destnum=*(int *)dest_final;
				int fromnum=(int)((AX *)from_final)->ptr;
				if(destnum<fromnum){
					flag=-1;
				}
				if(destnum>fromnum){
					flag=1;
				}
				if(destnum==fromnum){
					flag=0;
				}
			}
			if(ffrom==0&&fdest==1){
				int destnum=(int)((AX *)dest_final)->ptr;
				int fromnum=*(int *)from_final;
				if(destnum<fromnum){
					flag=-1;
				}
				if(destnum>fromnum){
					flag=1;
				}
				if(destnum==fromnum){
					flag=0;
				}
			}
			if(ffrom==1&&fdest==1){
				int destnum=(int)((AX *)dest_final)->ptr;
				int fromnum=(int)((AX *)from_final)->ptr;
					if(destnum<fromnum){
					flag=-1;
				}
				if(destnum>fromnum){
					flag=1;
				}
				if(destnum==fromnum){
					flag=0;
				}
			}
		 }
	}
	else if(d==11){	//��ת������֮ǰд�� 
		//���� 
		int input;
		printf("in:\n");
		scanf("%d",&input);//������һ�������� 
		if(fdest==0){
			*(int *)dest_final=input;//�洢�����ݼĴ�����
		}else {
			((AX *)dest_final)->ptr=(short)input;//�洢����ַ�Ĵ�����ָ����ڴ���
			if(((AX *)dest_final)->memptr==-1){
				curPtr=writeAs(Memory,((AX *)dest_final)->ptr,dataleft,ModifyShort);//����ҪС���ڴ�ĸ������ 	
			}else{
				writeAs(Memory,((AX *)dest_final)->ptr,((AX *)dest_final)->memptr,ModifyShort);
			}
		}
	}else if(d==12){
		int outputdata;
//		printf("���Դ�ӡ��ֵ��\n"); 
		if(fdest==0){
			outputdata=*(int *)dest_final; //�����ֵ�������� 
		}else if(fdest==1){
			outputdata=((AX *)dest_final)->ptr;//�����ֵ�ڵ�ַ�� 
		}
		printf("out:%d\n",outputdata);
	}
	return curPtr;
} 
int writeAs(char Memory[],short data,int dataleft,char ModifyShort[]){
	int bin=getBinary(data);
//	printf("д��ǰ��bin:%d",bin); 
	sprintf(ModifyShort,"%d",bin);
//	printf("д��ǰ���ַ���:\n"); 
//	for(int i=0;i<strlen(ModifyShort);i++){
//		printf("%c",ModifyShort[i]);
//	}
//	printf("\n");
	return PutInMemory(Memory,ModifyShort,dataleft);//д����ַ���������� 
}
int PutInMemory(char Memory[],char beput[],int dataleft){//������һ���ܷŽ����Ľӿ�,д���ڴ��ʱ��,����ط������� 
	int dataright;
	int putindex=strlen(beput)-1;
//	printf("beput=%s,dataleft=%d,strlen(beput)=%d\n",beput,dataleft,strlen(beput));//beputΪ16400 
	for(dataright=dataleft+15;dataright>dataleft;dataright--){//��ȡ�Ⱥ�,��Ϊȡ���Ժ�Ż��������,ȷʵ��ֻ����15λ��ֵ 
		if(putindex>=0){
			Memory[dataright]=beput[putindex];//���ȥ�� ��ȷ�� printf("Put:%c\n",beput[putindex-1]);
//			printf("Memory[i]:%c\n",Memory[dataright]);
			putindex--;	
		}else {
			Memory[dataright]='0';                //printf("dataright=%d,Memory:%c\n",dataright,Memory[dataright]); 
		}
	}
	Memory[dataright]='0';//��16λ�� 
//	printf("�������ڴ��������ת�Ƶ���ַ������:"); 
//	for(int i=dataright;i<dataright+16;i++){
//		printf("%c",Memory[i]);
//	}
//	printf("�ڴ�,��ӡ����\n");
	return dataright+16;
}
int anlaysis(char dir[]){//����:����ָ��  ����ֵ:ִ�е��ǵڼ���ָ�� 
	char Whattodo[9]={'\0'};//��һ��:����ָ��Ҫ��ʲô 
	subStringByPos(dir,Whattodo,0,8,0);
//	printf("dir=%s,whattodo=%s\n",dir,Whattodo);//֤��whattodo��ȷ 
	char instructs[13][9]={"00000000","00000001","00000010","00000011","00000100","00000101","00000110","00000111","00001000","00001001",
	"00001010","00001011","00001100"};//ͣ�� ���� �� �� ��	//�� �� �� �� �Ƚ�	//��ת ���� ��� //ֱ��ö�ٶ�ά�����Ǻð취 
	int temp;int index;
	for(int i=0;i<13;i++){
		temp=strcmp(Whattodo,instructs[i]);
		if(temp==0){
			index=i;
			break;
		}
	}
	return index; 
}
void printDirect(char dir[][35],int size){//����:��ӡ����ε��ڴ�
	printf("����δ�ӡ��ʼ\n");
	for(int i=0;i<size;i++){
		printf("%lld ",getTenth(dir[i],strlen(dir[i])));
		if(i%7==0&&i!=0){
			printf("\n");
		}
	}
	printf("\n"); 
	printf("����δ�ӡ����\n");
} 
int readDIR(FILE *fp,char Memory[],int size){//�������i��ʾ��ȡ�˼���ָ�� 
	fp=fopen("test.txt","r");
	if(fp==NULL){
		printf("�ļ��򲻿����ļ�·������!\n"); 
		return -1;
	}
	int i;
	char ch;//�ļ��ж�ȡ���ַ� 
	int index=0;
	while((ch=fgetc(fp))!=EOF){
		Memory[index]=ch;
		index++;
		if(ch=='\n'){
			i++;
		}
	}
	Memory[index]='\n';//������һ���ַ�������һ�����з�,�����ȡ 
	fclose(fp);
	return i;
}
int getNegation(char binary[],int size){//ȡ��
	char c;
	for(int i=0;i<size;i++){
		c=binary[i];
		if(c=='0'){
			binary[i]='1';
		}else if(c=='1'){
			binary[i]='0';
		}
	}
	int tenth=getTenth(binary,16);//ȡ����һ:�����Ƽ�һ ���ǲ���˵����Ҫ������ת��Ϊ10����,��ת��Ϊ2���Ʋ���
	return getBinary(tenth+1);
}
int getBinary(int n){//��ȡ�������� 
    int binaryNumber = 0;
    int remain, i = 1;
 
    while (n!=0){
        remain = n%2;
        n /= 2;
        binaryNumber += remain*i;
        i *= 10;
    }
    return binaryNumber;
}
long long getTenth(char binary[],int size){//��ȡʮ������ 
	long long tenth=0;
	for(int i=0;i<size;i++){
		if(binary[i]=='\n'||binary[i]=='\0'){
			break;
		}
		tenth=tenth*2+binary[i]-'0';
	}
	return tenth;	
}
int divideInstruct(char buffer[],int size,char dir[]){//����ָ���ǰ16λ�ͺ�16λ 
	//����һ������:2����ת��Ϊ10����
	int i=16;
	char binary[17];
	subStringByPos(buffer,dir,0,16,0);
//	dir[16]='\0';//�ַ��������Ͻ����������ַ��� 
	subStringByPos(buffer,binary,16,32,0);
	//�����������
	int result;
	char my_bin[16];
	if(binary[0]=='0'){
		result=getTenth(binary,16);
	}else if(binary[0]=='1'){
		int bin=getNegation(binary,16);//ȡ����1,���䷴,��װһ������ 
		sprintf(my_bin,"%d",bin);//sprintf�������ַ���ת��Ϊint���� 
		result=0-getTenth(my_bin,strlen(my_bin));
	}
//	printf("%d\n",result);
	return result;
}
void printDataArea(char Memory[],int start){//�ѵ�:���ڴ�Ĳ��� 
	int i,data,index=0;
	char p[17];
	printf("�������ӡ��ʼ\n");
	for(i=DATASTART;i<DATASTART+4096;i+=16){
		subStringByPos(Memory,p,i,i+16,0);//printf("data:%s\n",p);
		data=getTenth(p,17);
		printf("%d ",data);index++;
		if(index%16==0){
			printf("\n");
		}
	}
	printf("�������ӡ����;\n"); 
}
void printInfo(int *dataAx[],AX* axptrs[],int ir,int flag,int ip){
	printf("\n");
	printf("ip = %d\n",ip);
	printf("flag = %d\n",flag);
	printf("ir = %d\n",ir);
	printf("ax1 = %d ax2 = %d ax3 = %d ax4 = %d\n",*dataAx[0],*dataAx[1],*dataAx[2],*dataAx[3]);
	printf("ax5 = %d ax6 = %d ax7 = %d ax8 = %d\n",axptrs[0]->data,axptrs[1]->data,axptrs[2]->data,axptrs[3]->data);
	printf("\n");
}
