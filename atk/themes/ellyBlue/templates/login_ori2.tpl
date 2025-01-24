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

<div id="container-login">
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
</div>
