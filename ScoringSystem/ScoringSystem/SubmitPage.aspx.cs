using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.IO;

public partial class SubmitPage : System.Web.UI.Page
{
    DateTime Deadline = DateTime.Parse("2017-11-26 23:33:45.340"); //deadline của server
    protected void Page_Load(object sender, EventArgs e)
    {
        DeadlineTime.Text = "Giờ hết hạn: " + Deadline.ToString("yyyy-mm-dd HH:mm:ss.fff");
    }

    protected void Submit_Button_Click(object sender, EventArgs e)
    {
        DateTime SubmitTime = DateTime.Now;
        if(SubmitTime > Deadline)
        {
            SubmissionNotification.Text = "Xin lỗi, bạn đã nộp với hạn quá trễ! Thời gian đã nộp là: " + SubmitTime.ToString("yyyy-mm-dd HH:mm:ss.fff");
        }
        else
        {
            SubmissionNotification.Text = "Xin vui lòng đợi...";
            string MSSV = MSSVInput.Text;
            if(MSSV == "")
            {
                SubmissionNotification.Text = "Bạn chưa cung cấp mã số sinh viên!";
            }
            else if(!Directory.Exists(Server.MapPath("Submission/" + MSSV)))
            {
                SubmissionNotification.Text = "Không tồn tại sinh viên này!";
            }
            else
            {
                if (!CodeUpload.HasFile)
                {
                    SubmissionNotification.Text = "Bạn chưa chọn file nào! Xin hãy chọn các file có đuôi .h hay .cpp";
                }
                else
                {
                    //convert to approriate name
                    char[] TimeChar = SubmitTime.ToString("yyyy-mm-dd HH:mm:ss.fff").ToCharArray();
                    TimeChar[10] = '_'; TimeChar[13] = '~'; TimeChar[16] = '~'; TimeChar[19] = ',';
                    string Time = new string(TimeChar);
                    //create additional directories
                    string MainDir = "Submission/" + MSSV + "/" + Time + "/" + "source";
                    string source = Server.MapPath("Submission/" + MSSV + "/" + Time + "/" + "source"); Directory.CreateDirectory(source);
                    string build = Server.MapPath("Submission/" + MSSV + "/" + Time + "/" + "build"); Directory.CreateDirectory(build);
                    string log = Server.MapPath("Submission/" + MSSV + "/" + Time + "/" + "build"); Directory.CreateDirectory(log);
                    string output = Server.MapPath("Submission/" + MSSV + "/" + Time + "/" + "output"); Directory.CreateDirectory(output);
                    foreach (HttpPostedFile UploadedFile in CodeUpload.PostedFiles)
                    {
                        //add files
                        string FileName = MainDir + "/" +  UploadedFile.FileName;
                        string FilePath = MapPath(FileName);
                        CodeUpload.SaveAs(FilePath);
                        SubmissionNotification.Text = "Tập tin gửi thành công!";
                    }
                    //add to text file
                    File.AppendAllText(MapPath("NewSubmission.txt"), MSSV + "/" + Time + Environment.NewLine);
                }
            }

        }
        
    }
}