{literal}
    <style type="text/css">
        body {
        {/literal}
/*        background: #E8EEF1;*/
        /*background: #ABCDEF;*/
        {literal}
        }
    </style>
{/literal}

<!-- <div id="container-login">
    <div id="login-top">&nbsp;</div>
    <div id="login-top-logo">
        <img src="{$themedir}/images/login-logo.png" width="100" height="100" />
        <span id="product">{atktext app_shorttitle}</span>
    </div>
    <div id="login-content">
        <div id="login-form">
            <form action="{$formurl}" method="post">
                {if isset($auth_max_loginattempts_exceeded)}
                    {$auth_max_loginattempts_exceeded}
                {else}
                    {$atksessionformvars}
                {if isset($auth_mismatch)}<span class="error">{$auth_mismatch}</span><br>{/if}
            {if isset($auth_account_locked)}<span class="error">{$auth_account_locked}</span><br>{/if}

            <div id="title">{atktext app_title}</div>
            <div><input id="username" type="text" name="auth_user" /></div>
            <div><input id="password" type="password" name="auth_pw" /></div>
            <div><input id="submit" type="submit" value="{atktext login}" /></div>
            {/if}
    </form>
</div>
</div>
<div id="login-bottom">
    <div id="app_title">{atktext app_title}</div>
    <div id="copyright">Visio Informatika &copy; 2012</div>
</div>
</div> -->

{literal}
<style type="text/css">

    body {
        background-color: #FFFFFF;
        /*height: 700px;*/
    }

    img {
        border: none;
    }
    #btnLogin {
        padding: 0;
    }
    input:not([type="image"]) {
        background-color: transparent;
        border: none;
    }

    input:focus, select:focus, textarea:focus {
        background-color: transparent;
        border: none;
    }

    .textInputContainer {
        font-family: Arial, Helvetica, sans-serif;
        font-size: 11px;
        color: #666666;
    }

    #divLogin {
        background: transparent url('./atk/themes/ellyBlue/images/login.png') no-repeat center top;
        height: 520px;
        width: 100%;
        border-style: hidden;
        margin: auto;
        /*padding-left: 10px;*/
    }

    #divUsername {
        /*padding-top: 153px;*/
        padding-top: 146px;
        padding-left: 50%;
    }

    #divPassword {
        /*padding-top: 35px;*/
        padding-top: 24px;
        padding-left: 50%;
    }

    #username {
        width: 240px;
        border: 0px;
        background-color: transparent;
    }

    #password {
        width: 240px;
        border: 0px;
        background-color: transparent;
    }

    #username, #password {
        font-family: Arial, Helvetica, sans-serif;
        font-size: 11px;
        color: #666666;
        vertical-align: middle;
        /*padding-top:0;*/
        padding: 5px 0px 6px 5px;

    }
    
    #divLoginHelpLink {
        width: 270px;
        background-color: transparent;
        height: 20px;
        margin-top: 12px;
        margin-right: 0px;
        margin-bottom: 0px;
        margin-left: 50%;
    }

    #divLoginButton {
        padding-top: 2px;
        padding-left: 49.3%;
        float: left;
        width: 350px;
    }

    #btnLogin {
        background: url('./atk/themes/ellyBlue/images/Login_button.png') no-repeat;
        cursor:pointer;
        width: 94px;
        height: 26px;
        border: none;
        color:#FFFFFF;
        font-weight: bold;
        font-size: 13px;
        padding-bottom: 3px;
    }

    #btnLogin:hover {
        opacity: .8;
    }

    #divLink {
        padding-left: 230px;
        padding-top: 105px;
        float: left;
    }

    #divLogo {
        padding-left: 30%;
        padding-top: 70px;
    }

    #spanMessage {
        background: transparent url('./atk/themes/ellyBlue/images/mark.png') no-repeat;
        padding-left: 18px; 
        padding-top: 0px;
        color: #DD7700;
        font-weight: bold;
        position: absolute;
    }
    
    #logInPanelHeading{
        position:absolute;
        padding-top:100px;
        padding-left:49.5%;
        font-family:sans-serif;
        font-size: 15px;
        color: #544B3C;
        font-weight: bold;
    }
    
    .form-hint {
    color: #878787;
    padding: 4px 8px;
    position: relative;
    left:-254px;
}


    #divFooter {
        text-align: center;
        color: #5D5D5D;
    }

    #divFooter a {
        color: #5D5D5D;
    }
    
    #spanCopyright, #spanSocialMedia {
        padding: 20px 10px 10px 10px;
    }
    
    #spanSocialMedia a img {
        border: none;
    }


</style>

{/literal}

<div id="divLogin">
    <div id="divLogo">
        <img src="{$themedir}/images/logo.png">
    </div>

    <form id="frmLogin" method="post" action="{$formurl}" autocomplete="off">
        <input name="actionID" type="hidden">
        <input name="hdnUserTimeZoneOffset" id="hdnUserTimeZoneOffset" value="7" type="hidden">
        <div id="logInPanelHeading">LOGIN Panel</div>

        <div id="divUsername" class="textInputContainer">
            <input name="auth_user" id="username" type="text" placeholder="Username" required />
        </div>
        <div id="divPassword" class="textInputContainer">
            <input name="auth_pw" id="password" type="password" placeholder="Password" required />
        </div>
        <div id="divLoginHelpLink"></div>
        <div id="divLoginButton">
            <input name="Submit" class="button" id="btnLogin" value="{atktext login}" type="submit">
            <!-- <span id="spanMessage">Username cannot be empty</span> -->
        </div>
    </form>

    <div id="divLoginButton">
        {if isset($auth_max_loginattempts_exceeded)}
            {$auth_max_loginattempts_exceeded}  
        {else}
            {$atksessionformvars}
            {if isset($auth_mismatch)}<br /><br /><br /><span id="spanMessage">{$auth_mismatch}</span>{/if}
            {if isset($auth_account_locked)}<br /><br /><br /><span id="spanMessage">{$auth_account_locked}</span>{/if}
        {/if}
    </div>

</div>

<div id="divFooter">
    <span id="spanCopyright">
        Powered by <a href="http://www.nusacode.com" target="_blank" title="NusaCode" alt="NusaCode"><img src="{$themedir}/images/logo-nusacode83x35.png" align="middle"></a>
    </span>
    <span id="spanSocialMedia">
        <a href="http://www.facebook.com/VisioInformatika" target="_blank" title="@VisioInformatika" alt="@VisioInformatika">
            <img src="{$themedir}/images/facebook.png" align="middle"></a>&nbsp;
        <a href="http://twitter.com/VisInformatika" target="_blank" title="@VisInformatika" alt="@VisInformatika">
            <img src="{$themedir}/images/twiter.png" align="middle"></a>&nbsp;
    </span>
    <br class="clear">
</div>
