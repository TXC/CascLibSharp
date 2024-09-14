using CommunityToolkit.Maui.Core.Primitives;
using MudBlazor;

namespace CASCLibSharp.MAUIExample.Components.Pages
{
    public partial class Index
    {
        //ObservableCollection<CASCFile> CASCFiles = [];
        //List<CASCFileInfo> CASCFiles = [];
        List<CASCFileInfo> SelectedItems = [];
        CASCStorage? Storage { get; set; }

        DisplayProgress progress = null!;
        bool ShowProgress = false;
        MudDataGrid<CASCFileInfo> mudDataGrid { get; set; } = null!;

        async Task OpenStorageClick()
        {
            Storage?.Dispose();

            try
            {
                /*
                var result = await filePicker.PickAsync(new PickOptions
                {
                    PickerTitle = "Open CASC Storage",
                    FileTypes = new FilePickerFileType(new Dictionary<DevicePlatform, IEnumerable<string>>
                    {
                        { DevicePlatform.WinUI, new[] { "idx" } },
                    })
                });
                */
                var result = await folderPicker.PickAsync();
                if (result.IsSuccessful)
                {
                    Storage = new CASCStorage(result.Folder.Path);

                    //CASCFiles.AddRange(Storage.Files);
                    //foreach (CASCFileInfo file in Storage.Files)
                    //{
                    //    CASCFiles.Add(new(file.FileName, file.FileSize, file.IsLocal));
                    //}

                    await mudDataGrid.ReloadServerData();
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error picking file: {ex.Message}");

                Snackbar.Add("Failed to open CASC Storage. Ensure that the storage is intact and exists at the given location.",
                             Severity.Error,
                             config =>
                             {
                                 config.RequireInteraction = true;
                                 config.ShowCloseIcon = true;
                             });


                Storage?.Dispose();
            }
        }

        async Task ExtractAllClick()
        {
            if (Storage?.Files.Length <= 0)
            {
                Snackbar.Add("No files provided to extract.",
                             Severity.Error,
                             config =>
                             {
                                 config.RequireInteraction = true;
                                 config.ShowCloseIcon = true;
                             });
                return;
            }

            Folder outputFolder;

            var result = await folderPicker.PickAsync();
            if (result.IsSuccessful)
            {
                outputFolder = result.Folder;
            }
            else
            {
                return;
            }

            ShowProgress = true;

            new Thread(async () => await SaveFilesToLocalStorage(outputFolder, [.. Storage?.Files]))
                      .Start();
        }

        async Task ExtractSelectedClick()
        {
            if (SelectedItems.Count <= 0)
            {
                Snackbar.Add("No files provided to extract.",
                             Severity.Error,
                             config =>
                             {
                                 config.RequireInteraction = true;
                                 config.ShowCloseIcon = true;
                             });
                return;
            }

            Folder outputFolder;

            var result = await folderPicker.PickAsync();
            if (result.IsSuccessful)
            {
                outputFolder = result.Folder;
            }
            else
            {
                return;
            }

            ShowProgress = true;

            new Thread(async () => await SaveFilesToLocalStorage(outputFolder, [.. SelectedItems]))
                      .Start();
        }

        async Task CloseStorageClick()
        {
            //CASCFiles.Clear();
            SelectedItems.Clear();
            Storage?.Dispose();
            Storage = null;
            await mudDataGrid.ReloadServerData();
        }

        /// <summary>
        /// Here we simulate getting the paged, filtered and ordered data from the server
        /// </summary>
        ///
        async Task<GridData<CASCFileInfo>> ServerDataFunc(GridStateVirtualize<CASCFileInfo> gridState, CancellationToken token)
        {
            if (Storage is null)
            {
                return new GridData<CASCFileInfo>
                {
                    TotalItems = 0,
                    Items = []
                };
            }
            IEnumerable<CASCFileInfo> data = Storage.Files;

            if (gridState.SortDefinitions.Count > 0)
            {
                var firstSort = gridState.SortDefinitions.First();
                data = firstSort.Descending
                    ? data.OrderByDescending(firstSort.SortFunc).ToList()
                    : data.OrderBy(firstSort.SortFunc).ToList();
            }

            if (gridState.FilterDefinitions.Any())
            {
                var filterFunctions = gridState.FilterDefinitions.Select(x => x.GenerateFilterFunction());
                data = data
                    .Where(x => filterFunctions.All(f => f(x)))
                    .ToList();
            }


            int totalItems = data.Count();

            var pagedData = data.Skip(gridState.StartIndex)
                                  .Take(gridState.Count)
                                  .ToArray();
            return new GridData<CASCFileInfo>
            {
                TotalItems = totalItems,
                Items = pagedData
            };
        }

        async Task SaveFilesToLocalStorage(Folder folder, List<CASCFileInfo> files)
        {
            var buffer = new byte[0x800000];

            progress.SetTotalProgressCount(files.Count);

            foreach (var file in files)
            {
                if (file.IsLocal)
                {
                    progress.SetMessage(string.Format("Extracting {0}...", Path.GetFileName(file.FileName)));
                    progress.SetProgressCount(file.FileSize / 0x800000);

                    string newPath = Path.Combine(folder.Path, file.FileName);
                    string dirPath = Path.GetDirectoryName(newPath) ?? throw new IOException();
                    
                    Directory.CreateDirectory(dirPath);

                    try
                    {
                        using MemoryStream stream = new ();
                        using CASCFileStream? input = (Storage?.OpenFile(file.FileName)) ?? throw new IOException();
                        while (true)
                        {
                            var fileSaverResult = await fileSaver.SaveAsync(newPath, input);
                            if (fileSaverResult.IsSuccessful != true)
                            {
                                throw fileSaverResult.Exception;
                            }

                            if (progress.IncrementProgress())
                            {
                                break;
                            }

                            if (input.Length < 0x800000)
                            {
                                break;
                            }

                            /*
                            var bytesRead = input.Read(buffer, 0, 0x800000);
                            stream.Write(buffer);

                            if (progress.IncrementProgress())
                            {
                                break;
                            }

                            if (bytesRead < 0x800000)
                            {
                                break;
                            }
                            */
                        }
                        /*
                        //using var output = File.Create(newPath);
                        var fileSaverResult = await fileSaver.SaveAsync(newPath, stream);
                        if (fileSaverResult.IsSuccessful != true)
                        {
                            throw fileSaverResult.Exception;
                        }
                        */
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine(ex);
                    }
                }

                if (progress.IncrementTotalProgress())
                {
                    break;
                }
            }

            progress.Complete();
        }

        void SelectedItemsChanged(HashSet<CASCFileInfo> items)
        {
            SelectedItems = items.ToList();
        }
    }
}
