clear; close all; clc;

% storage_matrix_sng_frame = [32.2 33 41.1 121.6 ; ...
%                             93 102.5 124.4 367.8 ; ...
%                             252 275.8 349.8 1024 ];
% 
% storage_matrix_all_frame = [176.2 196.4 229.1 668; ...
%                             497.1 546.8 673.7 1946.6; ...
%                             1331.2 1433.6 1843.2 5222.4];
                        
storage_matrix_sng_frame = [32.2 93 252; ...
                            33 102.5 275.8;...
                            41.1 124.4 349.8;...
                            121.6 367.8 1024];
                        
storage_matrix_all_frame = [176.2 497.1 1331.2;...
                            196.4 546.8 1433.6; ...
                            229.1 673.7 1843.2; ...
                            668 1946.6 5222.4];

data_pts = 1:3;
plot(data_pts, storage_matrix_all_frame, '^-')
grid on;
xlabel('Video Size');
ylabel('Size (in MB)');
title('Total storage size for different resolutions');
legend('JPG Grayscale','JPG RGB', 'PNG Grayscale', 'PNG RGB');
% figure;
% plot(data_pts, storage_matrix_all_frame, '*-');

% %Log file format (all values in milliseconds)
% %Total time, Video Load, Open Chk, IM Grab, IM Retrieve, RGB 2 GRAY, Resize, 
% %62434      240         0           60      11          9           1
% %Individual Img Proc log file format  (all values in microseconds)
% %Grab Time, Retrieve time, Color, Resize
% 
% num_data_per_iter = 5; % Tells the number of data points collected for each iteration
% num_frame_skips = 0; %[5 10 20 50 100 200]
% tot_iters = 5;
% num_of_frames = 6960;
% % path = 'C:\Users\Rohit\Documents\GitHub\video-query\sys_perf\logs\ind_img_proc_224x224_color.txt';
% path = 'C:\Users\Rohit\Documents\GitHub\video-query\sys_perf\logs\log_no_imshow.txt';
% 
% %Read the log file, scan to get each data point and close the file
% file_p = fopen(path,'r');
% myContent = textscan(file_p, '%s');
% fclose(file_p);
% 
% data_matrix = zeros(num_data_per_iter, num_of_frames);
% final_val_mat = zeros(tot_iters, 6);
% count = 1;
% curr_index = 1;
% for i=1:tot_iters
%     while(strcmp(myContent{1,1}(curr_index),'New_iter') ~= 0)
%        curr_index = curr_index + 1; 
%     end
%     curr_index = curr_index + 1;
%     count = curr_index;
%     for j=curr_index:(curr_index + num_of_frames)
%         for k=1:num_data_per_iter
%             data_matrix(j, k) = str2double(myContent{1,1}(count));
%             count = count + 1;
%         end
%     end
%     curr_index = count;
%     figure;
%     plot(data_matrix);
%     while(strcmp(myContent{1,1}(curr_index),'Value') ~= 0)
%        curr_index = curr_index + 1; 
%     end
%     curr_index = curr_index + 1;
%     count = curr_index;
%     for j = 1: 6
%         final_val_mat(i,j) = str2double(myContent{1,1}(count));
%     end
% end
% 
% % plot_titles = {'Total time', 'Video Load', 'Check Opened', 'IM Grab', 'IM Retrieve', 'RGB2Gray', 'Resize'};
% plot_titles = {'IM Grab', 'IM Retrieve', 'RGB2Gray', 'Resize'};
% figure;
% for j=1:size(data_matrix,1)
%     subplot(2,2,j);
% %     plot(data_matrix(j,2:end));
% %     xlabel('Iteration #');
% %     ylabel('Elapsed time (uSec)');
%     histogram(data_matrix(j,1:end))
%     xlabel('Time for 1 operation (usec)');
%     ylabel('Number of frames');
%     title(plot_titles{1,j});
% end
