<%@ Page Language="C#" AutoEventWireup="true" CodeFile="SubmitPage.aspx.cs" Inherits="SubmitPage" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title></title>
</head>
<body>
    <form id="form1" runat="server">
    <div>
    
        Nộp bài<br />
        <asp:Label ID="DeadlineTime" runat="server" Text=""></asp:Label>
        <br />
        MSSV:
        <asp:TextBox ID="MSSVInput" runat="server" Width="186px"></asp:TextBox>
        <br />
        <asp:FileUpload ID="CodeUpload" AllowMultiple="true" runat="server" />
        <br />
        <asp:Label ID="SubmissionNotification" runat="server" Text=""></asp:Label>
    
        <br />
        <asp:Button ID="Submit" runat="server" Text="Nộp bài" OnClick="Submit_Button_Click" />
    
    </div>
    </form>
</body>
</html>
