import os

def makeSampleTxt(samplekind, sampleDir, sampleSize=(24, 24)):
    posFileName = 'posSample.txt'
    negFileName = 'negSample.txt'
    if samplekind=='pos':
        if os.path.isdir(sampleDir):
            filelist = os.listdir(sampleDir)
            sampleTxt = file(os.path.join(sampleDir, posFileName), 'w')

        for tfile in filelist:
            sampleTxt.write(os.path.join(sampleDir, tfile) + " 1 0 0 "+repr(sampleSize[0])+" "+repr(sampleSize[0])+"\n")
    else:
        if os.path.isdir(sampleDir):
            filelist = os.listdir(sampleDir)
            sampleTxt = file(os.path.join(sampleDir, negFileName), 'w')

        for tfile in filelist:
            sampleTxt.write(os.path.join(sampleDir, tfile) + "\n")

def main():
    path = "/home/yangzheng/myProgram/test"
    sampleSize = (24, 24)
    makeSampleTxt('pos', os.path.join(path, 'posSample'), sampleSize)
    makeSampleTxt('neg', os.path.join(path, 'negSample'))

if __name__=='__main__':
    main()

