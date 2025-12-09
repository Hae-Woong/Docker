param (
    [string]$WorkspaceDir = "${PWD}",          # Host directory to use as the GitHub repo root
    [string]$ContainerName = "canoe-container" # Container name
)

# 1. Run Docker container (background, interactive)
Write-Host "Starting Docker container '$ContainerName'..."
docker run -it -d `
    --name $ContainerName `
    -v "D:\__action-runner_1-3_Docker\LVL1\_work\Docker\Docker" `
    canoe-se-window:1.0 `
    powershell

# 2. Wait a few seconds (allow container to start)
Start-Sleep -Seconds 5

# 3. Execute CANoe SE commands inside the container
Write-Host "Running CANoe SE environment-make and test-unit-make inside container..."
docker exec -it $ContainerName powershell -Command "
    # 1) Make environment
    & environment-make.exe' -o C:\workspace\compiled-environment-and-tests -A Win32 -s L1_vECU C:\workspace\environment-make\venvironment.yaml;

    # 2) Make test units
    & 'C:\Program Files\Vector CANoe SE\test-unit-make.exe' `
        -e C:\workspace\compiled-environment-and-tests\L1_vECU.venvironment `
        -o C:\workspace\compiled-environment-and-tests `
        C:\workspace\tests\auto\auto.vtestunit.yaml `
        C:\workspace\tests\basic\basic.vtestunit.yaml
"

# 4. Stop and remove the container
Write-Host "Stopping and removing container '$ContainerName'..."
docker stop $ContainerName
docker rm $ContainerName

Write-Host "CANoe SE build completed. Check results in '$WorkspaceDir\compiled-environment-and-tests'"
