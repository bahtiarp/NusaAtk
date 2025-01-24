<body class="easyui-layout">
    <div border="0" id="north" region="north" scrolling="no"  split="false">
        <div id="top-layout">
            <div id="top-layout-left">
                <img src="{$themedir}images/login-logo.png" height="56" width="56" alt="{atktext app_title}" title="{atktext app_title}" />{atktext app_title}
            </div>
            <div style="padding-top: 0px;" id="top-layout-right">
   
                
<table style="text-align:left;font-size: 11px;" >

    <tr><td rowspan="2"><a href="#" class="dropdown-toggle" data-toggle="dropdown" aria-expanded="false">
                                <i style="font-size: xx-large;color:white;margin:-10px;" class="fa fa-envelope"></i>
                                <span style="border-radius: 10px;" class="label label-default">4</span>
                            </a></td><td style="color:white;font-size: 12px;"><span id="user-info">{$user.firstname} {$user.lastname}</span><br><span id="client-name">{$user.clientName}</span></td><td rowspan="2"><a href="index.php?atklogout=1" target="{$logouttarget}">
                    <img id="logout" src="{$themedir}images/icon-logout.png" height="32" width="32" alt="{atktext logout}" title="{atktext logout}" /> 
                       </a></td></tr>
        <tr><td><span class="label label-default" style="color:white;" id="client-info"></span></td></tr>
</table>
                       
                <!-- <a href="javascript:void(0);" onclick="window.open('{$linkuid}','EditProfile','width=570,height=400,menubar=no,location=no,status=no,resizable=no,toolbar=no,titlebar=no')">
                    <img id="user" src="{$themedir}images/icon-user.png" height="32" width="32" alt="Edit profile" title="Edit profile" /> 
                </a> -->

            </div>
        </div>
    </div>

    <div id="menu" region="west" border="false" split="false" title="<i>Menu Category</i>" style="width:250px;padding:0px; overflow: auto !important;">
        <!--        <div class="easyui-accordion" fit="true" border="false">
                    <div title="Menu Utama" style="overflow:auto;width:250px;padding:5px;" selected="true"> -->
        {$menu}
        <!--            </div>
                    <div title="User Info"  >
                        <iframe name="main" scrolling="no" frameborder="0"  src="userInfo.php" style="width:100%;height:100%;" ></iframe>
                    </div>
                </div>-->
    </div>

    <div  region="south" split="false" style="height:30px;" scrolling="no">
<div id="bottom-layout">Powered by <a href="http://www.nusacode.com" target="_blank">NusaCode</a></div> 
    </div>

    <div region="center">
        <div id="tt" class="easyui-tabs" fit="true" border="false">
            <div title="Welcome" icon="icon-reload" closable="false" >
                <iframe name="main" scrolling="auto" frameborder="0" src="welcome.php" style="width:100%;height:99.6%;" ></iframe>
            </div>
        </div>
    </div>


    {literal}
        <script type="text/javascript">
            $(function(){
            //        $('body').layout('collapse','east');

                    $('#tt').tabs({
                            fit: true
                    });

                    $('#tt1').tree('collapseAll');

            });

            var index = 0;
            function addTab(title, url){
                if ($('#tt').tabs('exists', title)){
                    $('#tt').tabs('select', title);
                } else {
                    var content = '<iframe scrolling="auto" frameborder="0"  src="'+url+'" style="width:100%;height:99.6%;"></iframe>';
                    $('#tt').tabs('add',{
                        title:title,
                        content:content,
                        closable:true
                    });
                }
            }
     
    $(document).ready(function(){
        function pull(){
            $.ajax({
            url: "index.php?atknodetype=messaging.quicksmsatkaction=getCredit",
            dataType: 'json',
            cache: false,
            type: "get",
            error: function () {},
            success: function (result) {
                $("#client-name").html(result.data.clientName);
                console.log(result);
                if(result.data.level < 3){ $("#client-info").html("Credits:<b>"+result.data.balance+" | Plafon:"+result.data.plafonBalance+" | Expired:"+result.data.expiredDate);}
                else
                { $("#client-info").html("Credits:<b>"+result.data.balance+" | Expired:"+result.data.expiredDate);}
               
            }
        });   
        }
        pull();
        setInterval(function(){
        pull();
        }, 60000);

        
        
    });
        
        </script>
    {/literal}