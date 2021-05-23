# encoder_app
App to encoder the video files in S3

# Requirements
- AWS CLI V2 : https://docs.aws.amazon.com/cli/latest/userguide/install-cliv2.html
- AWS SDK for C++ : https://docs.aws.amazon.com/sdk-for-cpp/v1/developer-guide/sdk-from-source.html
- Open CV : https://opencv.org/releases/
- CMake : https://github.com/Kitware/CMake/releases/tag/v3.18.6

# Setup
- Create S3 bucket in your region
- Upload any sample mp4 video file into the S3 bucket
- Update encode.cpp main method with the bucket and file names
- Create IAM Policy by referring the file iam_s3_policy.json. Replace the "<<YourS3BucketName>>" with your actual bucket name
- Create IAM User with Programmatic Access, and attch the above policy to the user. Store the generated credentials securely.
- Run "aws configure" and provide credentials as generated in the above step. Select your region and output type as json.


# Build and Run
- mkdir build
- cd build
- cmake ..
- make
- ./run_encode  Input_File_Key Input_Bucket_Name Output_File_Key Output_Bucket_Name AWS_Region
Ex: .\run_encode.exe SampleVideo_1280x720_5mb.mp4 test-encoding-app SampleVideo_1280x720_5mb_encoded.mp4 test-encoding-app ap-northeast-1
