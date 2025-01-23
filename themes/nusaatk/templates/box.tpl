<!--<div class="newboxtitle">{$title}</div>-->
<!--<div class="content-wrapper" style="min-height: 1969px;">-->


<div class="app-content-header"> <!--begin::heaader-->
    <div class="container-fluid">
        <div class="row">
            <div class="col-sm-6">
                <h3 class="mb-0">{$title}</h3>
            </div>
            <div class="col-sm-6">
                <ol class="breadcrumb justify-content-xl-end">
                    <li class="breadcrumb   breadcrumb-item"><a href="{$stacktrace[0].url}"><span
                                class="fa-solid fa-house"></span></a></li>
                </ol>

            </div>
        </div>
    </div>
</div> <!--end::header-->

<div class="app-content">
    <div class="container-fluid">
                {$content}
    </div>
</div>