{atkconfig var="companylogo" smartyvar="theme_logo"}
{atkconfig var="admin_logo" smartyvar="admin_logo"}
{atkconfig var="persondir_url" smartyvar="persondir"}
{atkconfig var="client_logo_compressed" smartyvar="dir_logo"}
{atkconfig var="user_photo_compressed" smartyvar="dir_foto"}
{atkconfig var="client_name" smartyvar="client_name"}
{atkconfig var="pp" smartyvar="pp"}



<aside class="app-sidebar bg-body-secondary shadow" data-bs-theme="light"> <!--begin::Sidebar Brand-->

    <div class="sidebar-brand"> <!--begin::Brand Link--> <a href="./" class="brand-link">
            <!--begin::Brand Image--> <img src="{atkthemeimg id="logo.png"}" alt="NusaATK Logo"
                class="brand-image opacity-75 "> <!--end::Brand Image--> <!--begin::Brand Text--> 
                <!-- <span class="brand-text fw-light">AdminLTE 4</span>  -->
            </div>
    <!--end::Sidebar Brand--> <!--begin::Sidebar Menu-->

    <div class="sidebar-wrapper" data-overlayscrollbars="host">
        <div data-overlayscrollbars-viewport="scrollbarHidden" style="margin-right: -16px; margin-bottom: -16px; margin-left: 0px; top: -8px; right: auto; left: -8px; width: calc(100% + 16px); padding: 8px;">
            <nav class="fw-bold"> <!--begin::Sidebar Menu-->
                <ul class="nav sidebar-menu flex-column" data-lte-toggle="treeview" role="menu" data-accordion="false">
                    {$content}
                </ul>
            </nav> <!--end::Sidebar Menu-->
        </div>
    </div>
</aside> <!--end::Sidebar Brand--> <!--begin::Sidebar Menu-->

<main class="app-main"> <!--begin::App Content Header-->
    