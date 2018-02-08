void h5_read_write_test(){
    h5_handler h5;
    h5.create_file("img.h5");
    Mat _img(3,3,CV_8UC1,Scalar(125));
//    imwrite("test_img.jpg", _img);
//    Mat _img = imread("test_img.jpg", CV_LOAD_IMAGE_GRAYSCALE);

    hsize_t * shape;
    vector<int> vshape;
    int rank;
    const int nImg = 10;
    if(_img.channels()==3){
        rank = 4;
        shape = new hsize_t[rank];
        shape[0] = nImg;
        shape[1] = static_cast<hsize_t>(_img.rows);
        shape[2] = static_cast<hsize_t>(_img.cols);
        shape[3] = 3;
        vshape.push_back(nImg);
        vshape.push_back(_img.rows);
        vshape.push_back(_img.cols);
        vshape.push_back(3);
    }
    if(_img.channels()==1){
        rank = 3;
        shape = new hsize_t[rank];
        shape[0] = nImg;
        shape[1] = static_cast<hsize_t>(_img.rows);
        shape[2] = static_cast<hsize_t>(_img.cols);
        vshape.push_back(nImg);
        vshape.push_back(_img.rows);
        vshape.push_back(_img.cols);
    }
    h5.create_dataset("TrainingSet", h5_data::NATIVE_UCHAR, rank, shape);
    h5_data * data = make_data_h5("img", h5_data::NATIVE_UCHAR,rank,vshape, static_cast<void*>(_img.data));
    hsize_t start[3]; // Start of hyperslab
    hsize_t stride[3]; // Stride of hyperslab
    hsize_t count[3];  // Block count
    hsize_t block[3];  // Block sizes
    start[0]  = 0; start[1]  = 0; start[2]  = 0;
    stride[0] = 1; stride[1] = 1; stride[2] = 1;
    count[0]  = 1; count[1]  = _img.rows; count[2] = _img.cols;
    block[0]  = 1; block[1]  = 1,block[2]  = 1;
    h5.dataspace->selectHyperslab( H5S_SELECT_SET, count, start, stride, block);
    h5.write_data(*data, *h5.dataspace);
}
void h5_test3(){
    h5_handler_c h5(string("isis.h5"));
    const hsize_t rank=2;
    hsize_t * shape = new hsize_t[rank];
    shape[0]=4;
    shape[1]=5;
    h5.create_dataset("data", h5_data::NATIVE_INT, rank, shape);
    hsize_t start[rank]={0,0},
            stride[rank]={1,1},
            count[rank]={3,3},
            block[rank]={1,1};
    hsize_t _dims[rank] = {3,3};
    h5_shape _shape(rank, _dims);
    int data[3][3];
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            data[i][j]=1;
    h5.write_data(start,stride,count,block,_shape,data);
}
void h5_test2(){
    hid_t file_id, dataset_id, memset_id, dataspace_id;
    const char * filename = "test.h5";
    const int rank=4;
    const int N = 4, S = 3, H = 2, W = 2;
    file_id = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    hsize_t dims[rank];
    dims[0]=N;dims[1]=S;dims[2]=H;dims[3]=W;

    dataspace_id = H5Screate_simple(rank, dims, NULL);
    dataset_id = H5Dcreate2(file_id, "data", H5T_NATIVE_INT, dataspace_id,H5P_DEFAULT, H5P_DEFAULT,H5P_DEFAULT);

    hsize_t start[rank]= {0,0,0,0},
            stride[rank]={1,1,1,1},
            count[rank]={1,S,H,W+1},
            block[rank]={1,1,1,1};

    hsize_t dimsmem[rank];
    dimsmem[0]=1;dimsmem[1]=S;dimsmem[2]=H;dimsmem[3]=W;
    memset_id = H5Screate_simple(rank,dimsmem,NULL);

//    int * data = new int[1*S*H*W];
    int data[1][S][H][W];
    for(int i=0;i<S;i++)
        for(int j=0;j<H;j++)
            for(int k=0;k<W;k++){
//                data[i*H*W+j*W+k]=i+1;
                data[0][i][j][k]=i+1;
            }
    for(int i=0;i<S;i++){
        for(int j=0;j<H;j++){
            for(int k=0;k<W;k++){
//                data[i*H*W+j*W+k]=i+1;
                std::cout<<data[0][i][j][k]<<',';
            }
            std::cout<<std::endl;
        }
    }

    H5Sselect_hyperslab(dataspace_id,H5S_SELECT_SET,start,stride,count,block);
    H5Dwrite(dataset_id,H5T_NATIVE_INT,memset_id,dataspace_id,H5P_DEFAULT,data);

    //file_id, dataset_id, memset_id, dataspace_id;
    H5Sclose(dataspace_id);
    H5Sclose(memset_id);
    H5Dclose(dataset_id);
    H5Fclose(file_id);
}

int h5_test1(){
    //继续研究
#define FILE        "subset.h5"
#define DATASETNAME "IntArray"
#define RANK      2

#define DIM0_SUB  3    // 子集的维度
#define DIM1_SUB  4

#define DIM0      8    // 原集的维度
#define DIM1      10
    // 先创建一个文件并写入原始数据集
    hid_t file_id;
    file_id = H5Fcreate(FILE, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    hsize_t dims[2];
    dims[0] = DIM0;
    dims[1] = DIM1;
    hid_t dataspace_id, dataset_id;
    dataspace_id = H5Screate_simple(RANK, dims, NULL);
    dataset_id = H5Dcreate2(file_id, DATASETNAME, H5T_STD_I32BE, dataspace_id,
                            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    // 创建写入数据集
    /* 写入文件中的数据为
    [1, 1, 1, 1, 1, 2, 2, 2, 2, 2;
    1, 1, 1, 1, 1, 2, 2, 2, 2, 2;
    1, 1, 1, 1, 1, 2, 2, 2, 2, 2;
    1, 1, 1, 1, 1, 2, 2, 2, 2, 2;
    1, 1, 1, 1, 1, 2, 2, 2, 2, 2;
    1, 1, 1, 1, 1, 2, 2, 2, 2, 2;
    1, 1, 1, 1, 1, 2, 2, 2, 2, 2;
    1, 1, 1, 1, 1, 2, 2, 2, 2, 2]
    */
    int i, j;
    int data[DIM0][DIM1];
    for (j = 0; j < DIM0; j++) {
        for (i = 0; i < DIM1; i++)
            if (i < (DIM1 / 2))
                data[j][i] = 1;
            else
                data[j][i] = 2;
    }

    herr_t status;
//    status = H5Dwrite(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL,
//                      H5P_DEFAULT, data);

    status = H5Sclose(dataspace_id);
    status = H5Dclose(dataset_id);
    status = H5Fclose(file_id);

    // ************ 对子集进行操作 *****************************
    // 打开文件中数据集，对子集进行编辑
    file_id = H5Fopen(FILE, H5F_ACC_RDWR, H5P_DEFAULT);
    dataset_id = H5Dopen2(file_id, DATASETNAME, H5P_DEFAULT);

    // 定义子集四大件，补偿，个数，间隔和块大小
    hsize_t     count[2];   // 块的大小
    hsize_t     offset[2];  // 补偿，即开始位置
    hsize_t     stride[2];  // 间隔
    hsize_t     block[2];   // 块的个数

    offset[0] = 1;
    offset[1] = 2;

    count[0] = DIM0_SUB;
    count[1] = DIM1_SUB;

    stride[0] = 1;
    stride[1] = 1;

    block[0] = 1;
    block[1] = 1;

    // 创建内存中的dataspce空间
    hsize_t dimsm[2];
    dimsm[0] = DIM0_SUB;
    dimsm[1] = DIM1_SUB;
    hid_t memspace_id;
    memspace_id = H5Screate_simple(RANK, dimsm, NULL);
    // 读取文件中dataset的dataspace空间
    dataspace_id = H5Dget_space(dataset_id);
    // 选取文件中空间的子集和内存空间相对应
    // stauts = H5Sselect_hyperslab(hid_t space_id, 要修改的space的句柄id
    //                            H5S_seloper_t op, 进行的操作
    //                              - H5S_SELECT_SET 替换已有选择
    //                              - H5S_SELECT_OR  交集
    //                              - H5S_SELECT_AND 并集
    //                              - H5S_SELECT_XOR 异或，反色操作
    //                              - H5S_SELECT_NOTB 减去B
    //                              - H5S_SELECT_NOTA 减去A
    //                        const hsize_t *start, 位置补偿
    //                       const hsize_t *stride, 间隙
    //                        const hsize_t *count, 块数
    //                        const hsize_t *block 块大小)
    status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, offset,
                                 stride, count, block);

    // 生成要填入子集的矩阵
    int sdata[DIM0_SUB][DIM1_SUB];
    for (j = 0; j < DIM0_SUB; j++) {
        for (i = 0; i < DIM1_SUB; i++)
            sdata[j][i] = 5;
    }

    // 文件中的子集写入数据
    /* 现在文件中的数据为
    [1, 1, 1, 1, 1, 2, 2, 2, 2, 2;
     1, 1, 5, 5, 5, 5, 2, 2, 2, 2;
     1, 1, 5, 5, 5, 5, 2, 2, 2, 2;
     1, 1, 5, 5, 5, 5, 2, 2, 2, 2;
     1, 1, 1, 1, 1, 2, 2, 2, 2, 2;
     1, 1, 1, 1, 1, 2, 2, 2, 2, 2;
     1, 1, 1, 1, 1, 2, 2, 2, 2, 2;
     1, 1, 1, 1, 1, 2, 2, 2, 2, 2]
    */
    status = H5Dwrite(dataset_id, H5T_NATIVE_INT, memspace_id,
                      dataspace_id, H5P_DEFAULT, sdata);

    status = H5Sclose(memspace_id);
    status = H5Sclose(dataspace_id);
    status = H5Dclose(dataset_id);
    status = H5Fclose(file_id);
    //************* 完成子集操作 *********************

    return 0;
}

void h5_test(){
    h5_handler h5;
    Vec_DataSet dataset;
    double * data = new double[3*4];
    int * data1 = new int[5*5*3];
    for(size_t i=0;i<3;i++)
        for(size_t j=0;j<4;j++){
            data[i*4+j] = 0.27;
//            std::cout<<data[i*4+j];
        }

    for(size_t i=0;i<5;i++)
        for(size_t j=0;j<5;j++){
            data1[(i*5+j)*3+0] = 5;
            data1[(i*5+j)*3+1] = 6;
            data1[(i*5+j)*3+2] = 7;
        }
    vector<int> shape0,shape1;
    shape0.push_back(3);
    shape0.push_back(4);
    shape1.push_back(5);
    shape1.push_back(5);
    shape1.push_back(3);
    dataset.push_back(make_data_h5("data_double", h5_data::NATIVE_DOUBLE,2,shape0,data));
    dataset.push_back(make_data_h5("data_int", h5_data::NATIVE_INT,3,shape1,data1));

    h5.create_h5_file("yz.h5", dataset);
    std::cout<<"h5 test finished"<<std::endl;
    delete data;
    delete data1;
}
