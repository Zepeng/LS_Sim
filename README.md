# LS_Sim

Simplified liquid scintillator simulation (No photon sensor) with options of Geant4 and Opticks for photon simulation.

## Getting started

To make it easy for you to get started with GitLab, here's a list of recommended next steps.

Already a pro? Just edit this README.md and make it your own. Want to make it easy? [Use the template at the bottom](#editing-this-readme)!

## Add your files

- [ ] [Create](https://docs.gitlab.com/ee/user/project/repository/web_editor.html#create-a-file) or [upload](https://docs.gitlab.com/ee/user/project/repository/web_editor.html#upload-a-file) files
- [ ] [Add files using the command line](https://docs.gitlab.com/ee/gitlab-basics/add-file.html#add-a-file-using-the-command-line) or push an existing Git repository with the following command:

```
~~cd existing_repo
git remote add origin https://code.ihep.ac.cn/huyuxiang/ls_sim.git
git branch -M main
git push -uf origin main~~
```

#Compilation
Compilation is configured with go.sh
./go.sh

#GPU configuration
Opticks is capable of using multiple GPUs, but this has not been implemented in this simulation. If multiple GPUs are available, it is necessary to set up CUDA_VISIBLE_DEVICES
export CUDA_VISIBLE_DEVICES=0 (or other number)

#run the executable
copied the executable from the build directory to current directory, then
./example -m MACRO_NAME -o 1 (configure simulation to use Opticks)

