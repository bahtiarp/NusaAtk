<head>
  <link href="./atk/themes/zune/styles/style.css" rel="stylesheet" type="text/css" media="all" />
  <link href="./atk/themes/zune/styles/login.css" rel="stylesheet" type="text/css" media="all" />
  <link href=".//atk/javascript/window/themes/default.css" rel="stylesheet" type="text/css" media="all" />
  <link href=".//atk/javascript/window/themes/zune.css" rel="stylesheet" type="text/css" media="all" />
</head>

<div id="login">

  <div class="main">
    <div class="left">{atkconfig var="logo" smartyvar="logo"}
      <img src="./atk/themes/zune/images/logo.gif" alt="{atktext app_title}" />
    </div>
{if isset($auth_max_loginattempts_exceeded)}
    <span class="error" id="atkloginattemptsexceeded">{$auth_max_loginattempts_exceeded}</span>
{else}
{if isset($auth_mismatch)}
    <span class="error">{$auth_mismatch}</span>
{elseif isset($auth_account_locked)}
    <span class="error">{$auth_account_locked}</span>
{/if}
    <div class="right">
      <div class="gradient">
        <b class="bx_gr_t"><b class="bdw_gr_1"></b><b class="bdw_gr_2"></b><b class="bdw_gr_3"></b><b class="bdw_gr_4"></b><b class="bdw_gr_5"></b><b class="bdw_gr_6"></b></b>
        <div class="bx_gr"><div class="sp_av"></div><div class="sp_fl"></div></div>
        <b class="bx_gr_b"><b class="bdw_gr_6"></b><b class="bdw_gr_5"></b><b class="bdw_gr_4"></b><b class="bdw_gr_3"></b><b class="bdw_gr_2"></b><b class="bdw_gr_1"></b></b>
      </div>
      <div class="form">
        <form action="{$formurl}" method="post">
          <div class="avatar">
            <b class="bx_av_t"><b class="bdw_av_1"></b><b class="bdw_av_2"></b></b><div><img src="./atk/themes/zune/images/default.jpg" alt="{atktext user}" /></div><b class="bx_av_b"><b class="bdw_av_2"></b><b class="bdw_av_1"></b></b>
          </div>
          <div class="fields">
{$atksessionformvars}
            <label class="username">{$userfield}</label><br />
            <label class="password">{atktext type_your_password}:<br /><b class="bx_pw_t"><b class="bdw_pw_1"></b><b class="bdw_pw_2"></b></b>{$passwordfield}<b class="bx_pw_b"><b class="bdw_pw_2"></b><b class="bdw_pw_1"></b></b></label>
          </div>
          <input name="login" class="login_btn" type="submit" title="{atktext login}" value="{atktext login}">{if ($auth_enablepasswordmailer)}<input name="login" class="send_btn" type="submit" title="{atktext password_forgotten}" value="{atktext password_forgotten}">{/if}
        </form>
      </div>
    </div>
{/if}
  </div>
  <div class="vr">&nbsp;</div>
  <div class="hd"><div class="hr">&nbsp;</div></div>
  <div class="ft"><div class="hr">&nbsp;</div></div>
  <div color="white" class="ft_txt"><h2 style= "font-size:14px; color:white;">Aplikasi Monitoring Kontrak&nbsp;</h2></div>
  <div style="z-index:100;position: absolute; bottom: 10px; left:10px;"><img src="{$themedir}/images/logoflexi.png"/></div>
</div><!-- end login -->
