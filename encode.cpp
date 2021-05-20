// Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
// SPDX - License - Identifier: Apache - 2.0 

#include <iostream>
#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <fstream>
#include <awsdoc/s3/s3_examples.h>
#include <sys/stat.h>
#include <aws/s3/model/PutObjectRequest.h>

/* ////////////////////////////////////////////////////////////////////////////
 * Purpose: Prints the beginning contents of a video file in a
 * bucket in Amazon S3, encode it and save it to s3
 *
 * Prerequisites: The bucket that contains the video file.
 *
 * Inputs:
 * - inputObjectKey: The name of the video file.
 * - fromBucket: The name of the bucket that contains the input video file.
 * - outputObjectKey: The name of the video file.
 * - toBucket: The name of the bucket that will contain the output video file.
 * - region: The AWS Region for the bucket.
 *
 * Outputs: true if the contents of the video file were retrieved and encoded
 * otherwise, false.
 * ///////////////////////////////////////////////////////////////////////// */

bool AwsDoc::S3::EncodeObject(const Aws::String& inputObjectKey, const Aws::String& fromBucket,
    const Aws::String& outputObjectKey, const Aws::String& toBucket, const Aws::String& region)
{
    Aws::Client::ClientConfiguration config;

    if (!region.empty())
    {
        config.region = region;
    }

    Aws::S3::S3Client s3_client(config);

    Aws::S3::Model::GetObjectRequest get_object_request;
    get_object_request.SetBucket(fromBucket);
    get_object_request.SetKey(inputObjectKey);

    Aws::S3::Model::PutObjectRequest put_object_request;
    put_object_request.SetBucket(toBucket);
    put_object_request.SetKey(outputObjectKey);

    Aws::S3::Model::GetObjectOutcome get_object_outcome =
        s3_client.GetObject(get_object_request);

    if (get_object_outcome.IsSuccess())
    {
        auto& retrieved_file = get_object_outcome.GetResultWithOwnership().
            GetBody();

        // Print a beginning portion of the video file.
        std::cout << "Beginning of file contents:\n";
        char file_data[255] = { 0 };
        retrieved_file.getline(file_data, 254);
        std::cout << file_data << std::endl;

        put_object_request.SetBody(get_object_request);

        Aws::S3::Model::PutObjectOutcome outcome =
            s3_client.PutObject(put_object_request);

        if (outcome.IsSuccess()) {

            std::cout << "Added object '" << outputObjectKey << "' to bucket '"
                << toBucket << "'.";
            return true;
        }
        else
        {
            std::cout << "Error: PutObject: " <<
                outcome.GetError().GetMessage() << std::endl;

            return false;
        }
    }
    else
    {
        auto err = get_object_outcome.GetError();
        std::cout << "Error: GetObject: " <<
            err.GetExceptionName() << ": " << err.GetMessage() << std::endl;

        return false;
    }
}

int main()
{
    Aws::SDKOptions options;
    Aws::InitAPI(options);
    {
        const Aws::String input_object_name = "SampleVideo_1280x720_5mb.mp4";
        const Aws::String input_bucket_name = "test-encoding-app";
        const Aws::String output_object_name = "SampleVideo_1280x720_5mb_encoded.mp4";
        const Aws::String output_bucket_name = "test-encoding-app";
        const Aws::String region = "ap-northeast-1";

        if (!AwsDoc::S3::EncodeObject(input_object_name, input_bucket_name, output_object_name, output_bucket_name, region))
        {
            return 1;
        }
    }
    Aws::ShutdownAPI(options);

    return 0;
}
