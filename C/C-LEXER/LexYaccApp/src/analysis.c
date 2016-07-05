#include "defs.h"

// 分析Create语句
int analysis_create(struct Createdef *_create){
    FILE *fp;
    int col_cnt=0;
    char path[128] = "./tables";

    if(access(path,0)==-1){
        mkdir(path,0777);        
    }

    strcat(path,"/sys.dat");

    if((fp = fopen(path,"w+")) == NULL){
        printf("open file failed.\n");
    }
    else
	{
        struct Createfieldsdef *_col = (struct Createfieldsdef *)malloc(sizeof(struct Createfieldsdef));
        _col = _create->cdef;
		printf("table:%s\n", _create->table);
        do{
			printf("field:%s\n", _col->col);
            // int 类型 
            if(_col->type == _INT){
				printf("filed type: INT\n");
                fprintf(fp,"%s %d %s int\n", _create->table, ++col_cnt, _col->col);
            }
            // char 类型 
            else if(_col->type == _CHAR){
				printf("filed type: CHAR\n");
                fprintf(fp,"%s %d %s char %d\n", _create->table, ++col_cnt, _col->col, _col->length);
            }
            _col = _col->next_cdef;
        }while(_col);
        free(_col);
        fflush(fp); 
    }

    return 0;
}

// 分析Insert语句
int analysis_insert(struct Insertdef *_insert){
    FILE *fp;
    char path[128] = "./tables/";
    struct Insertfieldsdef *temp;
    if(access(path,0)==-1){
        mkdir(path,0777);
    }
    strcat(path,_insert->table);
    temp = (struct Insertfieldsdef *)malloc(sizeof(struct Insertfieldsdef));
    temp = _insert->idef;

	printf("table:%s\n", _insert->table);

    do{
		/* output */
        if(temp->type == _INT){
            printf("%d ",temp->intcont);
        }
        else if(temp->type == _CHAR)
		{
            printf("%s ",temp->strcont);
        }
        puts(path);

        if((fp = fopen(path,"a+")) == NULL)
        {
            printf("Failed to open %s",path);
        }
        else
		{
            if(temp->type == _INT){
                fprintf(fp,"%d ",temp->intcont);
            }
            else if(temp->type == _CHAR){
                fprintf(fp,"%s ",temp->strcont);
            }
            fflush(fp);
        }
        temp = temp->next_idef;
    }while(temp);
    fprintf(fp,"\n"); 

    free(temp);
    fflush(fp);

    return 0;
}

// 分析Select语句
int analysis_select(struct Selectstruct *_select){
	char buf[1024]; 

	FILE *fp;
    char path[128] = "./tables/";
    struct Selectedfields *_field;
	struct Selectedtables *_table;

    printf("\nEnter select!\n");  return 0;

    strcat(path,_table->table);
	if ((fp = fopen(path, "r")) == NULL) {
		printf("open file failed.\n");
		return -1;
    }

	while (fgets(buf, sizeof(buf), fp) != NULL) {
		printf("%s", buf);
	}
    
    return 0;
}

int printf_Selectcondition(struct Conditions *temp){

	if (!temp) return 0;

    if(temp->left){
        printf("(L)");
        printf_Selectcondition(temp->left);

    }
    else{
        if(temp->type==0||temp->type==1){
            if (temp->str_value) printf("%s ",temp->str_value);
        }
        else if(temp->type==2){
            printf("%d ",temp->int_value);
        }
        else{
            printf("left type error!\n");
        }
    }
    if(temp->right){
        printf("%s ",temp->comp_op);
        printf("(R)");
        printf_Selectcondition(temp->right);
    }

    return 0;
}
