<body class="easyui-layout">
	<div border="0" id="north" region="north" scrolling="no"  split="true" style="border:0px;overflow: hidden;height:70px;">

	<table bgcolor="#abcdef" border="0" width="100%" id="table1" cellspacing="0" cellpadding="0">
	<tr>
<td style="background:transparent url({$themedir}/images/logoflexi.png) no-repeat  30px;height:70px;width:160px;border:0px;">
</td>
		<td valign="top" border="0" style="text-align:Center;font-family : arial;font-size:18px;color:darkblue;"><br>{atktext app_title}</td>
		
		<td align="right" valign="top" style="background: url({$themedir}/images/logo-telkom.png) no-repeat;height:70px;width:160px;border:0px;"  ></td>
	</tr>
</table>	
	
         <!--   <a href="index.php"><img src="{atkthemeimg logo.png}" alt="Logo Achievo" /></a> -->
        </div>
		
	<div id="menu" region="west" border="false"  split="false" title="<i>Menu Category</i>" style="width:250px;padding:0px;">
		
		<div class="easyui-accordion" fit="true" border="false">
			<div title="Menu Utama" style="overflow:auto;width:250px;padding:5px;" selected="true"> 
				{$menu}
			</div>
			<div title="User Info"  >
			<iframe name="main" scrolling="no" frameborder="0"  src="userInfo.php" style="width:100%;height:100%;" ></iframe>
			</div>
			<div title="Links"></div>
					
		</div>
	</div>
	
		<!--
	<div region="east" split="true" title="News" style="width:200px;padding:10px;">east region</div>
	-->
	
	<div region="south" split="false" style="border:0px;height:20px;background:transparent;">
		<table bgcolor="#1C527E" border="0" width="100%" id="table1" cellspacing="0" cellpadding="0">
			<tr>
				<td align="left">
				</td>
				<td id="bottom" style="text-align:Center;font-family : arial;font-size:16px;color:lightblue;">Copyright &copy PT Telekomunikasi Indonesia, Tbk</td>
				<td align="right">
				</td>
			</tr>
		</table>	
	</div>
		
		
		<div region="center"  style="overflow:hidden;">
			<div id="tt" class="easyui-tabs" fit="true" border="false">
				<div title="Welcome" icon="icon-reload" closable="true" >
					<iframe name="main" scrolling="auto" frameborder="0"    src="welcome.php" style="width:100%;height:100%;" ></iframe>
					
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
            var content = '<iframe scrolling="auto" frameborder="0"  src="'+url+'" style="width:100%;height:100%;"></iframe>';
            $('#tt').tabs('add',{
                title:title,
                content:content,
                closable:true
            });
        }
    }
</script>
{/literal}